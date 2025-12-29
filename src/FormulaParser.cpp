#pragma once
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

    double totalQty = 0;
    double totalPxQty = 0.0;

    for (const auto& e : execList) {
        totalQty += e.quantity;
        totalPxQty += e.price * e.quantity;
    }

    agg.quantity = totalQty;
    agg.price = (totalQty !=0) ? (totalPxQty / totalQty) : 0.0; // weighted avg
    return agg;
}

//extern "C" FORMULAPARSER_API const char* __cdecl ProcessJson(const char* jsonInput, char* outBuffer, int bufferSize)
//extern "C" FORMULAPARSER_API void __cdecl ProcessJson(const char* jsonInput, char* outBuffer, int bufferSize)
//extern "C" void __cdecl ProcessJson(const char* jsonInput, char* outBuffer, int bufferSize)
//void ProcessJson(const char* jsonInput, char* outBuffer, int bufferSize)
//{
//    std::lock_guard<std::mutex> lock(g_mutex);
//    g_result.clear();
//    if (!jsonInput) {
//        g_result = "Error: null input";
//        /*return g_result.c_str();*/
//    }
//
//    Document d;
//    if (d.Parse(jsonInput).HasParseError()) {
//        g_result = "Error: invalid JSON";
//        /*return g_result.c_str();*/
//    }
//
//    std::priority_queue<OrderExecutionData> Executions;
//    std::vector<PlanData> Plans;
//    std::vector<ExecutionResult> results;
//    g_result = "{ \"Results\":[\n";
//    bool first = true;
//
//    makeOrder(d, Executions);
//    makePlans(d, Plans);
//    std::unordered_map<std::string, std::vector<OrderExecutionData>> orderGroups;
//   
//    //Build the execution list from the priority queue because queue will not keep execution after poping
//    std::vector<OrderExecutionData> allExecs;
//    while (!Executions.empty()) {
//        allExecs.push_back(Executions.top());
//        Executions.pop();
//    }
//    //group executions based on orderId
//  /*  for (const auto& exec : allExecs) {
//        orderGroups[exec.orderId].push_back(exec);
//    }*/
//    for (const auto& exec : allExecs) {
//        auto& group = orderGroups[exec.orderId];
//
//        if (group.empty()) {
//            group.push_back(exec);  // first entry for this order
//        }
//        else {
//            // compare ExecutionId values (convert to number if needed)
//            long long currentId = std::stoll(group[0].executionId);
//            long long newId = std::stoll(exec.executionId);
//
//            if (newId > currentId) {
//                group[0] = exec;  // replace with higher execution
//            }
//        }
//    }
//    for (const auto& plan : Plans) {
//        if (plan.planType == 2) {
//            // ========= PER EXECUTION =========
//            for (const auto& exec : allExecs) {
//                std::pair<bool, double> ans{ false, 0.0 };
//
//                switch (plan.formulaType) {
//                case EN_FormulaType_Tabular: {
//                    FeeFormula::TabularMethod tabularMethod(plan.formula_final);
//                    ans = tabularMethod.evaluate(exec);
//                    break;
//                }
//                case EN_FormulaType_Tiered: {
//                    TieredMethod tieredMethod(plan.formula_final);
//                    ans = tieredMethod.evaluate(exec);
//                    break;
//                }
//                case EN_FormulaType_Raw: {
//                    RawParser rawMethod(plan.formula_final);
//                    ans = rawMethod.Evaluate(exec);
//                    break;
//                }
//                }
//              //  g_result += "{ Id: " + exec.id + ",\nPlanId: " + plan.id + ",\nMonthlyVolume: " + std::to_string(exec.monthlyVolume) + ",\nFees: " + std::to_string(ans.second) + "},\n";
//                if (!first) g_result += ",\n";   // <-- comma before every element except first
//                first = false;
//                g_result += "  { "
//                    "\"ExecutionId\": \"" + exec.id + "\", "
//                    "\"OrderId\": \"" + exec.orderId + "\", "
//                    "\"AccountValue\": \"" + exec.accountValue + "\", "
//                    "\"Date\": \"" + exec.date + "\", "
//                    "\"PlanId\": \"" + plan.id + "\", "
//                    /*"\"MonthlyVolume\": " + std::to_string(exec.monthlyVolume) + ", "*/
//                    "\"Fees\": " + std::to_string(ans.second) +
//                    " }";
//            }
//        }
//        else if (plan.planType == 1) {
//            // ========= PER ORDER =========
//            for (auto& [orderId, execList] : orderGroups) {
//                if (execList.empty()) continue;
//
//                OrderExecutionData aggregated = aggregateExecutions(execList);
//
//                std::pair<bool, double> ans{ false, 0.0 };
//                switch (plan.formulaType) {
//                case EN_FormulaType_Tabular: {
//                    FeeFormula::TabularMethod tabularMethod(plan.formula_final);
//                    ans = tabularMethod.evaluate(aggregated);
//                    break;
//                }
//                case EN_FormulaType_Tiered: {
//                    TieredMethod tieredMethod(plan.formula_final);
//                    ans = tieredMethod.evaluate(aggregated);
//                    break;
//                }
//                case EN_FormulaType_Raw: {
//                    RawParser rawMethod(plan.formula_final);
//                    ans = rawMethod.Evaluate(aggregated);
//                    break;
//                }
//                }
//                //results.push_back({ aggregated.id, plan.id, std::to_string(aggregated.monthlyVolume), ans.second });
//               // g_result += "{ Id: " + aggregated.id + ",\nPlanId: " + plan.id + ",\nMonthlyVolume: " + std::to_string(aggregated.monthlyVolume) + ",\nFees: " + std::to_string(ans.second) + "},\n";
//                if (!first) g_result += ",\n";
//                first = false;
//                g_result += "  { "
//                    "\"Id\": \"" + aggregated.id + "\", "
//                    "\"OrderId\": \"" + aggregated.orderId + "\", "
//                    "\"ExecutionId\": \"" + aggregated.executionId + "\", "
//                    "\"AccountValue\": \"" + aggregated.accountValue + "\", "
//                    "\"Date\": \"" + aggregated.date + "\", "
//                    "\"PlanId\": \"" + plan.id + "\", "
//                    //"\"MonthlyVolume\": " + std::to_string(aggregated.monthlyVolume) + ", "
//                    "\"Fees\": " + std::to_string(ans.second) +
//                    " }";
//            }
//        }
//       }
//    g_result += "\n]}";
//    if (outBuffer != nullptr && bufferSize > 0)
//    {
//        memset(outBuffer, 0, bufferSize); // clear previous content
//        strncpy_s(outBuffer, bufferSize, g_result.c_str(), bufferSize - 1);
//    }
//
//    strncpy_s(outBuffer, bufferSize, g_result.c_str(), bufferSize - 1);
//
//    //return g_result.c_str();
//}




extern "C" FORMULAPARSER_API bool isValidExpression(const char* expr)
{
    try
    {
        RawParser parser(expr);
        return parser.IsCompiled();
    }
    catch (std::exception&)
    {
        return false;
    }
}
extern "C" __declspec(dllexport) BSTR __cdecl ProcessJson(BSTR jsonInput)
//std::string ProcessJson(BSTR jsonInput)
{
    std::lock_guard<std::mutex> lock(g_mutex);

    if (!jsonInput)
        return SysAllocString(L"Error: null input");

    // Convert BSTR (UTF-16) to std::string (UTF-8) for RapidJSON
    int len = WideCharToMultiByte(CP_UTF8, 0, jsonInput, -1, nullptr, 0, nullptr, nullptr);
    std::string jsonUtf8(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, jsonInput, -1, jsonUtf8.data(), len, nullptr, nullptr);

    Document d;
    if (d.Parse(jsonUtf8.c_str()).HasParseError())
        return SysAllocString(L"Error: invalid JSON");

        std::priority_queue<OrderExecutionData> Executions;
    std::vector<PlanData> Plans;
    std::vector<ExecutionResult> results;
    std::string  jsonResult = "{ \"Results\":[\n";
    bool first = true;

    makeOrder(d, Executions);
    makePlans(d, Plans);
    std::unordered_map<std::string, std::vector<OrderExecutionData>> orderGroups;
   
    //Build the execution list from the priority queue because queue will not keep execution after poping
    std::vector<OrderExecutionData> allExecs;
    while (!Executions.empty()) {
        allExecs.push_back(Executions.top());
        Executions.pop();
    }
    //group executions based on orderId
    //for (const auto& exec : allExecs) {
    //    orderGroups[exec.orderId].push_back(exec);
    //}
    for (const auto& exec : allExecs) {
        auto& group = orderGroups[exec.orderId];

        if (group.empty()) {
            group.push_back(exec);  // first entry for this order
        }
        else {
            // compare ExecutionId values (convert to number if needed)
            long long currentId = std::stoll(group[0].executionId);
            long long newId = std::stoll(exec.executionId);

            if (newId > currentId) {
                group[0] = exec;  // replace with higher execution
            }
        }
    }
    for (const auto& plan : Plans) {
        if (plan.planType == 2) {
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
              
                if (!first) jsonResult += ",\n";   
                first = false;
                jsonResult += "  { "
                    "\"ExecutionId\": \"" + exec.executionId + "\", "
                    "\"OrderId\": \"" + exec.orderId + "\", "
                    "\"AccountValue\": \"" + exec.accountValue + "\", "
                    "\"Date\": \"" + exec.date + "\", "
                    "\"PlanId\": \"" + plan.id + "\", "
                    "\"MonthlyVolume\": " + std::to_string(exec.monthlyVolume) + ", "
                    "\"Fees\": " + std::to_string(ans.second) +
                    " }";
            }
        }
        else if (plan.planType == 1) {
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
               // g_result += "{ Id: " + aggregated.id + ",\nPlanId: " + plan.id + ",\nMonthlyVolume: " + std::to_string(aggregated.monthlyVolume) + ",\nFees: " + std::to_string(ans.second) + "},\n";
                if (!first) jsonResult += ",\n";
                first = false;
                jsonResult += "  { "
                    "\"Id\": \"" + aggregated.id + "\", "
                    "\"OrderId\": \"" + aggregated.orderId + "\", "
                    "\"ExecutionId\": \"" + aggregated.executionId + "\", "
                    "\"AccountValue\": \"" + aggregated.accountValue + "\", "
                    "\"Date\": \"" + aggregated.date + "\", "
                    "\"PlanId\": \"" + plan.id + "\", "
                    "\"MonthlyVolume\": " + std::to_string(aggregated.monthlyVolume) + ", "
                    "\"Fees\": " + std::to_string(ans.second) +
                    " }";
            }
        }
       }
    jsonResult += "\n]}";


    // Convert std::string (UTF-8) back to BSTR (UTF-16)
    int wslen = MultiByteToWideChar(CP_UTF8, 0, jsonResult.c_str(), -1, nullptr, 0);
    BSTR resultBSTR = SysAllocStringLen(nullptr, wslen - 1);
    MultiByteToWideChar(CP_UTF8, 0, jsonResult.c_str(), -1, resultBSTR, wslen);

    //return resultBSTR;
    return resultBSTR;
}
