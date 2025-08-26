// This file contains the 'main' function

#include "RawParser.h"
#include "TabularParser.h"
#include "Timer.hpp"
#include "iostream"


bool isValidExpression(const char* expr)
{
	//puts(expr);
	try
	{
		RawParser parser(expr);
		bool isCompiled = parser.IsCompiled();
		if (isCompiled)
		{
			//std::cout<< "Compilation Successful!" << std::endl;
			return true;
		}
		else
		{
			//std::cout << "Compilation Un-Successful!" << std::endl;
			return false;
		}
	}
	catch (std::exception)
	{
		return false;
	}
}
int main()
{
	//if (json.GetMember("FormulaType") == 1)
	//{
	//	std::string str_ = json["Formula"];
	//	//TabularParser tabularMethod(str_1);
	//	/*for (execution : executions)
	//	{
	//		auto result = tabularMethod.Evaluate(execution);
	//	}*/


	//}
	//std::pair<bool,double> ans;


	//std::cout << "RawMethod:\n";
	//{
	//	Timer time_;
	//	for (int i = 0; i < 1; i++)
	//	{
	//		
	//		ans = rawMethod.Evaluate(data);
	//	}
	//}
	//std::cout << "RAW METHOD ANS: " << ans.second << std::endl;

	
}

//extern "C" __declspec(dllexport) bool __stdcall isValidExpression(const char* expr);