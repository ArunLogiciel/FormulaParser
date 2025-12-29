//#pragma once
//#define NOMINMAX
//#define WIN32_LEAN_AND_MEAN
//#include <comdef.h>
//#ifdef FORMULAPARSER_EXPORTS
//#define FORMULAPARSER_API __declspec(dllexport)
//#else
//#define FORMULAPARSER_API __declspec(dllimport)
//#endif

////extern "C" FORMULAPARSER_API const char* __cdecl  ProcessJson(const char* jsonInput, char* outBuffer, int bufferSize);
////extern "C" FORMULAPARSER_API void __stdcall  ProcessJson(const char* jsonInput, char* outBuffer, int bufferSize);
//void ProcessJson(const char* jsonInput, char* outBuffer, int bufferSize);
//std::string ProcessJson(std::string inputJson)
//extern "C" FORMULAPARSER_API BSTR __stdcall ProcessJson(BSTR jsonInput);
//extern "C" FORMULAPARSER_API bool isValidExpression(const char* expr);



#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <comdef.h>

#ifdef FORMULAPARSER_EXPORTS
#define FORMULAPARSER_API __declspec(dllexport)
#else
#define FORMULAPARSER_API __declspec(dllimport)
#endif

extern "C" {
    FORMULAPARSER_API BSTR __stdcall ProcessJson(BSTR jsonInput);
    FORMULAPARSER_API bool __stdcall isValidExpression(const char* expr);
}
