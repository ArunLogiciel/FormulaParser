#pragma once

#include "BusinessObjects.h"
#include <exprtk/exprtk.hpp>

using symbol_table_t = exprtk::symbol_table<double>;
using expression_t = exprtk::expression<double>;
using parser_t = exprtk::parser<double>;

//WARNING: This class is not thread-safe.
class RawParser
{
private:
	IdentifiableVariables vars;
	std::string formula;
	expression_t expression;
	parser_t parser;
	bool isCompiled;

	//Private Methods
	void PopulateSymbolTable();
	void ParseFormula();

public:
	explicit RawParser(std::string recvdFormula);
	std::pair<bool, double> Evaluate(const OrderExecutionData& data);
	bool IsCompiled();
	RawParser() = delete;
};
