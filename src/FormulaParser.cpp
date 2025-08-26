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
    std::queue<OrderExecutionData> Executions;
    std::vector<PlanData> Plans;
	std::vector<ExecutionResult> results;

    makeOrder(d, Executions);
    makePlans(d, Plans);

	while (!Executions.empty()) {
		OrderExecutionData exec = Executions.front();
		Executions.pop();
		double totalFee = 0.0;
		for (const auto& plan : Plans) {
			std::pair<bool, double> ans{ false, 0.0 };
			switch (plan.formulaType) {
			case 1: {
				FeeFormula::TabularMethod tabularMethod(plan.formula_final);
				ans = tabularMethod.evaluate(exec);
				break;
			}
			case 2: {
				TieredMethod tieredMethod(plan.formula_final);
				ans = tieredMethod.evaluate(exec);
				break;
			}
			case 3: {
				RawParser rawMethod(plan.formula_final);
				
				ans = rawMethod.Evaluate(exec);
				break;
			}
			}
			totalFee += ans.second;
		}
		results.push_back({ exec.executionId, totalFee });
	}

	Document outputDoc;
	outputDoc.SetArray();
	auto& alloc = outputDoc.GetAllocator();

	for (const auto& r : results) {
		Value obj(kObjectType);
		obj.AddMember("ExecutionId", Value(r.executionId.c_str(), alloc), alloc);
		obj.AddMember("TotalFee", r.totalFee, alloc);
		outputDoc.PushBack(obj, alloc);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	outputDoc.Accept(writer);

	g_result = buffer.GetString();
	return g_result.c_str();

}
