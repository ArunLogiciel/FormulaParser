#pragma once

#ifdef FORMULAPARSER_EXPORTS
#define FORMULAPARSER_API __declspec(dllexport)
#else
#define FORMULAPARSER_API __declspec(dllimport)
#endif

extern "C" {
    FORMULAPARSER_API const char* ProcessJson(const char* jsonInput);
}
