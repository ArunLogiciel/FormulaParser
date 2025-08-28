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
#include <JsonParser.h>


using namespace rapidjson;

// we need a static buffer to safely return const char*
static std::string g_result;
static std::mutex g_mutex;

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

	while(!Executions.empty()) {
		OrderExecutionData exec = Executions.top();
		Executions.pop();
	
		for (const auto& plan : Plans) {
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

			g_result += "{ ExecutionId: " + exec.executionId + ",\nPlanId: " + plan.id + ",\nFees: " + std::to_string(ans.second) + "},\n";
		}
		results.push_back({ exec.executionId, totalFee });
	}

	g_result = g_result + "]";
	return g_result.c_str();

}
