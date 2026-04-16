#pragma once
#include "RawParser.h"
#include "Interfaces/MethodInterface.h"
#include "rapidjson/document.h"
#include <boost/algorithm/string.hpp>
//WARNING: This class is not thread-safe.
RawParser::RawParser(std::string recvdFormula) : isCompiled(false)
{
	formula = recvdFormula;
	ParseFormula();
	PopulateSymbolTable();
	isCompiled_1 = parser.compile(formula, expression);
	isCompiled = parser.compile(expr_string, expression);
}
std::pair<bool, double> RawParser::Evaluate(const OrderExecutionData& data)
{
	vars.copy(data);
	PopulateSymbolTable();
	expression.value();
	if (!isfinite(vars.Fees))
	{
		vars.Fees = 0;
	}
	return std::make_pair(true, vars.Fees);
}

void RawParser::PopulateSymbolTable()
{
	symbol_table_t symbol_table;
	symbol_table.add_variable("Penny", vars.Penny);
	symbol_table.add_variable("Price", vars.Price);
	symbol_table.add_variable("Quantity", vars.Quantity);
	symbol_table.add_variable("MonthlyVolume", vars.MonthlyVolume);
	symbol_table.add_variable("AfterHours", vars.AfterHours);
	symbol_table.add_variable("BeforeHours", vars.BeforeHours);
	symbol_table.add_variable("Type", vars.Type);
	symbol_table.add_variable("Fee", vars.Fees);// Need to check if this is applicable: Fees
	symbol_table.add_stringvar("ExchangeType", vars.ExchangeType);
	symbol_table.add_stringvar("Route", vars.Route);
	symbol_table.add_stringvar("LIQ", vars.LIQ);
	symbol_table.add_stringvar("LOT", vars.LOT);
	symbol_table.add_stringvar("ExecBroker", vars.ExecBroker);
	symbol_table.add_stringvar("Contra", vars.Contra);
	symbol_table.add_stringvar("DST", vars.DST);
	symbol_table.add_stringvar("Tape", vars.Tape);
	symbol_table.add_stringvar("Time", vars.Time);
	symbol_table.add_stringvar("InternalLiq", vars.InternalLiq);
	symbol_table.add_stringvar("InternalRoute", vars.InternalRoute);
	symbol_table.add_stringvar("Side", vars.Side);
	symbol_table.add_stringvar("UnderlyingSymbol", vars.UnderlyingSymbol);	//Newly Added for Supporting PropReports Plan
	symbol_table.add_variable("MPIDMonthlyVolume", vars.MPIdMonthlyVolume);
	symbol_table.add_variable("FirmMonthlyVolume", vars.FirmMonthlyVolume);
	symbol_table.add_stringvar("LastMarket", vars.LastMarket);
	expression.register_symbol_table(symbol_table);
}

bool RawParser::IsCompiled()
{
	return isCompiled;
}
bool RawParser::IsCompiled_1()
{
	return isCompiled_1;
}
void RawParser::ParseFormula()
{
	if (formula.empty())
	{
		throw std::exception("Raw Formula Is Empty - Unable To Parse");
	}

	for (size_t i = 0; i < formula.length(); i++)
	{
		if (formula[i] == '\n' || formula[i] == '\r\n' || formula[i] == '\r' || formula[i] == '\n')
		{
			formula[i] = ' ';
		}

		else if ((i < formula.length() - 4) &&
			(formula[i] == '\\' && formula[i + 1] == 'r' && formula[i + 2] == '\\' && formula[i + 3] == 'n'))
		{
			formula[i] = ' ';
			formula.erase(i + 1, 3); // Removing next three chars of \r\n
		}
	}



	//for (size_t i = 0; i < formula.length(); i++)
	//{
	//	if (formula[i] == '\n' || formula[i] == '\r\n' || formula[i] == '\r' || formula[i] == '\n')
	//	{
	//		formula[i] = ' ';
	//	}

	//	else if ((i < formula.length() - 4) &&
	//		(formula[i] == '\\' && formula[i + 1] == 'r' && formula[i + 2] == '\\' && formula[i + 3] == 'n'))
	//	{
	//		formula[i] = ' ';
	//		formula.erase(i + 1, 3); // Removing next three chars of \r\n
	//	}
	//}

	rapidjson::Document document;
	document.Parse(formula.c_str());
	if (!document["MethodStr"].IsNull())
	{
		expr_string = "";
		expr_string = document["MethodStr"].GetString();
		return;
	}

}