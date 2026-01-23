// This file contains the 'main' function

#include "RawParser.h"
#include "TabularParser.h"
#include "Timer.hpp"
#include "iostream"
#include "FormulaParser.h"


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

	//// Prepare output buffer
	std::string jsonData = "";
	const int bufferSize = 10240;
	std::vector<char> outBuffer(bufferSize, 0);

	// Call your formula parser function
	//ProcessJson(jsonData);
   // ProcessJson(jsonData, outBuffer.data(), 10240);
	const char* expr = R"(
if(LastMarket == 'A')
{
        Fee := mul(Quantity, 0.36/0)/0;
}
else  /* price >= 1 */
{
        Fee := mul(Quantity, 0.45);
}
)";
	bool result = isValidExpression(expr);
	// Print whatever the function wrote into the buffer
	/*std::cout << "Output:\n" << outBuffer.data() << std::endl;*/
	std::cout << result << std::endl;
	std::cout << "Done." << std::endl;
	return 0;
}

//extern "C" __declspec(dllexport) bool __stdcall isValidExpression(const char* expr);

