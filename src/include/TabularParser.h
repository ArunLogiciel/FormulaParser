#pragma once

#include "TabularMethod.h"

class TabularParser
{
private:
	FeeFormula::TabularMethod method;

public:
	auto Evaluate(const OrderExecutionData& data)
	{
		auto fee = method.evaluate(data);
		return fee;
	}

	TabularParser(std::string formula) : method(formula) 
	{
		//Left Blank;
	}
};
