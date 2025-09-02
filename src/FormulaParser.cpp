#include "FormulaParser.h"
#include "TabularMethod.h"
#include "TieredMethod.h"
#include "RawParser.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <JsonParser.h>

using namespace rapidjson;

// we need a static buffer to safely return const char*
static std::string g_result;
static std::mutex g_mutex;
OrderExecutionData aggregateExecutions(const std::vector<OrderExecutionData>& execList) {
    OrderExecutionData agg = execList.back(); // start with last execution

    int64_t totalQty = 0;
    double totalPxQty = 0.0;

    for (const auto& e : execList) {
        totalQty += e.execQty;
        totalPxQty += e.price * e.execQty;
    }

    agg.quantity = totalQty;
    agg.price = (totalQty > 0) ? (totalPxQty / totalQty) : 0.0; // weighted avg
    return agg;
}

extern "C" FORMULAPARSER_API const char* ProcessJson(const char* jsonInput)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    g_result.clear();

    if (!jsonInput) {
        g_result = "Error: null input";
        return g_result.c_str();
    }

    Document d;
    if (d.Parse(jsonInput).HasParseError()) {
        g_result = "Error: invalid JSON";
        return g_result.c_str();
    }

    std::priority_queue<OrderExecutionData> Executions;
    std::vector<PlanData> Plans;
    std::vector<ExecutionResult> results;
    g_result = "";

    makeOrder(d, Executions);
    makePlans(d, Plans);
    static std::unordered_map<std::string, std::vector<OrderExecutionData>> orderGroups;
   
    //Build the execution list from the priority queue because queue will not keep execution after poping
    std::vector<OrderExecutionData> allExecs;
    while (!Executions.empty()) {
        allExecs.push_back(Executions.top());
        Executions.pop();
    }
    //group executions based on orderId
    for (const auto& exec : allExecs) {
        orderGroups[exec.orderId].push_back(exec);
    }

    for (const auto& plan : Plans) {
        if (plan.planType == 1) {
            // ========= PER EXECUTION =========
            for (const auto& exec : allExecs) {
                std::pair<bool, double> ans{ false, 0.0 };

                switch (plan.formulaType) {
                case EN_FormulaType_Tabular: {
                    FeeFormula::TabularMethod tabularMethod(plan.formula_final);
                    ans = tabularMethod.evaluate(exec);
                    break;
                }
                case EN_FormulaType_Tiered: {
                    TieredMethod tieredMethod(plan.formula_final);
                    ans = tieredMethod.evaluate(exec);
                    break;
                }
                case EN_FormulaType_Raw: {
                    RawParser rawMethod(plan.formula_final);
                    ans = rawMethod.Evaluate(exec);
                    break;
                }
                }
                g_result += "{ Id: " + exec.id + ",\nPlanId: " + plan.id + ",\nMonthlyVolume: " + std::to_string(exec.monthlyVolume) + ",\nFees: " + std::to_string(ans.second) + "},\n";
            }
        }
        else if (plan.planType == 2) {
            // ========= PER ORDER =========
            for (auto& [orderId, execList] : orderGroups) {
                if (execList.empty()) continue;

                OrderExecutionData aggregated = aggregateExecutions(execList);

                std::pair<bool, double> ans{ false, 0.0 };
                switch (plan.formulaType) {
                case EN_FormulaType_Tabular: {
                    FeeFormula::TabularMethod tabularMethod(plan.formula_final);
                    ans = tabularMethod.evaluate(aggregated);
                    break;
                }
                case EN_FormulaType_Tiered: {
                    TieredMethod tieredMethod(plan.formula_final);
                    ans = tieredMethod.evaluate(aggregated);
                    break;
                }
                case EN_FormulaType_Raw: {
                    RawParser rawMethod(plan.formula_final);
                    ans = rawMethod.Evaluate(aggregated);
                    break;
                }
                }
                //results.push_back({ aggregated.id, plan.id, std::to_string(aggregated.monthlyVolume), ans.second });
                g_result += "{ Id: " + aggregated.id + ",\nPlanId: " + plan.id + ",\nMonthlyVolume: " + std::to_string(aggregated.monthlyVolume) + ",\nFees: " + std::to_string(ans.second) + "},\n";
            }
        }
       }
    g_result += "]";
    return g_result.c_str();
}
