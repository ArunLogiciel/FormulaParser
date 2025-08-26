#include "TabularMethod.h"
#include "rapidjson/document.h"
//#include "logger/logger.hpp"
//#include "std::exception.h"

namespace FeeFormula
{
	TabularMethod::TabularMethod(std::string formula)
	{
		//std::cout << " 'Initializing Tabular Method'";
		parse_json(formula);
		make_tree(parsed_formulas);
	}

	std::pair<EqualityOperator, std::string> TabularMethod::parse_string_value(const std::string& str)
	{
		static const char* gt_op = ">";
		static const char* lt_op = "<";
		static const char* gteq_op = ">=";
		static const char* lteq_op = "<=";
		static const char* eq = "=";

		if (str.substr(0, 2) == gteq_op) 
		{
			return { EN_EqualityOperator_GreaterThanEqualsTo,str.substr(2) };
		}

		else if (str.substr(0, 2) == lteq_op) 
		{
			return { EN_EqualityOperator_LessThanEqualsTo,str.substr(2) };
		}

		else if (str.substr(0, 1) == gt_op) 
		{
			return { EN_EqualityOperator_GreaterThan,str.substr(1) };
		}

		else if (str.substr(0, 1) == lt_op) 
		{
			return { EN_EqualityOperator_LessThan,str.substr(1) };
		}

		else if (str.substr(0, 1) == eq) 
		{
			return { EN_EqualityOperator_EqualsTo,str.substr(1) };
		}

		else 
		{
			return { EN_EqualityOperator_EqualsTo,str };
		}
	}

	std::pair<EqualityOperator, double> TabularMethod::parse_double_value(const std::string& str)
	{
		static const char* gt_op = ">";
		static const char* lt_op = "<";
		static const char* gteq_op = ">=";
		static const char* lteq_op = "<=";
		static const char* eq = "=";

		if (str.substr(0, 2) == gteq_op)
		{
			return { EN_EqualityOperator_GreaterThanEqualsTo,std::stod(str.substr(2)) };
		}

		else if (str.substr(0, 2) == lteq_op)
		{
			return { EN_EqualityOperator_LessThanEqualsTo,std::stod(str.substr(2)) };
		}

		else if (str.substr(0, 1) == gt_op)
		{
			return { EN_EqualityOperator_GreaterThan,std::stod(str.substr(1)) };
		}

		else if (str.substr(0, 1) == lt_op)
		{
			return { EN_EqualityOperator_LessThan,std::stod(str.substr(1)) };
		}

		else if (str.substr(0, 1) == eq)
		{
			return { EN_EqualityOperator_EqualsTo,std::stod(str.substr(1)) };
		}

		else
		{
			return { EN_EqualityOperator_EqualsTo,std::stod(str) };
		}
	}
	std::pair<EqualityOperator, int64_t> TabularMethod::parse_int_value(const std::string& str)
	{
		static const char* gt_op = ">";
		static const char* lt_op = "<";
		static const char* gteq_op = ">=";
		static const char* lteq_op = "<=";
		static const char* eq = "=";

		if (str.substr(0, 2) == gteq_op)
		{
			return { EN_EqualityOperator_GreaterThanEqualsTo,std::stoll(str.substr(2)) };
		}

		else if (str.substr(0, 2) == lteq_op)
		{
			return { EN_EqualityOperator_LessThanEqualsTo,std::stoll(str.substr(2)) };
		}

		else if (str.substr(0, 1) == gt_op)
		{
			return { EN_EqualityOperator_GreaterThan,std::stoll(str.substr(1)) };
		}

		else if (str.substr(0, 1) == lt_op)
		{
			return { EN_EqualityOperator_LessThan,std::stoll(str.substr(1)) };
		}

		else if (str.substr(0, 1) == eq)
		{
			return { EN_EqualityOperator_EqualsTo,std::stoll(str.substr(1)) };
		}

		else
		{
			return { EN_EqualityOperator_EqualsTo,std::stoll(str) };
		}
	}

	void TabularMethod::make_tree(std::vector<std::vector<std::string>>& p_table)
	{
		//std::cout << " 'Populating Tabular Method's Tree Data Structure'";
		int i = 0;
		for (const auto& row : p_table)
		{
			//std::cout<<"sequence no. =" <<i <<std::endl;
			try
			{
				leaf_rule leaf;

				if (row.at(FormulaResult::EXECBROKER) != "")
				{
					condition execBroker;
					execBroker.term_ = FormulaResult::EXECBROKER;
					execBroker.op_ = EN_EqualityOperator_EqualsTo;
					execBroker.value_ = parse_string_values(row[FormulaResult::EXECBROKER]);
					leaf.conditions_.emplace_back(execBroker);
				}

				if (row.at(FormulaResult::CONTRA) != "")
				{
					condition contra;
					contra.term_ = FormulaResult::CONTRA;
					contra.op_ = EN_EqualityOperator_EqualsTo;
					contra.value_ = parse_string_values(row[FormulaResult::CONTRA]);
					leaf.conditions_.emplace_back(contra);
				}

				if (row.at(FormulaResult::ROUTE) != "")
				{
					condition route;
					route.term_ = FormulaResult::ROUTE;
					route.op_ = EN_EqualityOperator_EqualsTo;
					route.value_ = parse_string_values(row[FormulaResult::ROUTE]);
					leaf.conditions_.emplace_back(route);
				}

				if (row.at(FormulaResult::INTERNAL_ROUTE) != "")
				{
					condition internalRoute;
					internalRoute.term_ = FormulaResult::INTERNAL_ROUTE;
					internalRoute.op_ = EN_EqualityOperator_EqualsTo;
					internalRoute.value_ = parse_string_values(row[FormulaResult::INTERNAL_ROUTE]);
					leaf.conditions_.emplace_back(internalRoute);
				}

				if (row.at(FormulaResult::LIQ) != "")

				{
					condition liq;
					liq.term_ = FormulaResult::LIQ;
					liq.op_ = EN_EqualityOperator_EqualsTo;
					liq.value_ = parse_string_values(row[FormulaResult::LIQ]);
					leaf.conditions_.emplace_back(liq);
				}

				if (row.at(FormulaResult::DST) != "")
				{
					condition dst;
					dst.term_ = FormulaResult::DST;
					dst.op_ = EN_EqualityOperator_EqualsTo;
					dst.value_ = parse_string_values(row[FormulaResult::DST]);
					leaf.conditions_.emplace_back(dst);
				}

				if (row.at(FormulaResult::TAPE) != "")
				{
					condition tape;
					tape.term_ = FormulaResult::TAPE;
					tape.op_ = EN_EqualityOperator_EqualsTo;
					tape.value_ = parse_string_values(row[FormulaResult::TAPE]);
					leaf.conditions_.emplace_back(tape);
				}

				if (row.at(FormulaResult::PRICE) != "")
				{
					condition price;
					price.term_ = FormulaResult::PRICE;
					auto value = parse_double_value(row[FormulaResult::PRICE]);
					price.op_ = value.first;
					price.value_ = value.second;
					leaf.conditions_.emplace_back(price);
				}

				if (row.at(FormulaResult::QUANTITY) != "")
				{
					condition quantity;
					quantity.term_ = FormulaResult::QUANTITY;
					auto value = parse_int_value(row[FormulaResult::QUANTITY]);
					quantity.op_ = value.first;
					quantity.value_ = static_cast<int64_t>(value.second);
					leaf.conditions_.emplace_back(quantity);
				}

				if (row.at(FormulaResult::AFTERHOURS) != "")
				{
					condition afterHours;
					afterHours.term_ = FormulaResult::AFTERHOURS;
					afterHours.op_ = EN_EqualityOperator_EqualsTo;
					afterHours.value_ = string_to_bool(row[FormulaResult::AFTERHOURS]);
					leaf.conditions_.emplace_back(afterHours);
				}

				if (row.at(FormulaResult::PENNY) != "")
				{
					condition penny;
					penny.term_ = FormulaResult::PENNY;
					penny.op_ = EN_EqualityOperator_EqualsTo;
					penny.value_ = string_to_bool(row[FormulaResult::PENNY]);
					leaf.conditions_.emplace_back(penny);
				}

				if (row.at(FormulaResult::TIME) != "")
				{
					condition time;
					time.term_ = FormulaResult::TIME;
					auto value = parse_string_value(row[FormulaResult::TIME]);
					time.op_ = value.first;
					time.value_ = value.second;
					leaf.conditions_.emplace_back(time);
				}

				if (row.at(FormulaResult::TYPE) != "")
				{
					condition type;
					type.term_ = FormulaResult::TYPE;
					type.op_ = EN_EqualityOperator_EqualsTo;
					type.value_ = static_cast<int64_t>(std::stoi(row[FormulaResult::TYPE]));
					leaf.conditions_.emplace_back(type);
				}

				if (row.at(FormulaResult::INTERNAL_LIQ) != "")
				{
					condition internalLiq;
					internalLiq.term_ = FormulaResult::INTERNAL_LIQ;
					internalLiq.op_ = EN_EqualityOperator_EqualsTo;
					internalLiq.value_ = row[FormulaResult::INTERNAL_LIQ];
					leaf.conditions_.emplace_back(internalLiq);
				}

				if (row.at(FormulaResult::BEFOREHOURS) != "")
				{
					condition beforHours;
					beforHours.term_ = FormulaResult::BEFOREHOURS;
					beforHours.op_ = EN_EqualityOperator_EqualsTo;
					beforHours.value_ = string_to_bool(row[FormulaResult::BEFOREHOURS]);
					leaf.conditions_.emplace_back(beforHours);
				}

				if (row.at(FormulaResult::LOT) != "") {
					condition lot;
					lot.term_ = FormulaResult::LOT;
					lot.op_ = EN_EqualityOperator_EqualsTo;
					lot.value_ = row.at(FormulaResult::LOT);
					leaf.conditions_.emplace_back(lot);
				}

				if (row.at(FormulaResult::SIDE) != "")
				{
					condition side;
					side.term_ = FormulaResult::SIDE;
					side.op_ = EN_EqualityOperator_EqualsTo;
					side.value_ = static_cast<int64_t>(std::stoi(row.at(FormulaResult::SIDE)));
					leaf.conditions_.emplace_back(side);
				}

				if (row.at(FormulaResult::MONTHLY_VOLUME) != "")
				{

					condition monthlyVolume;
					monthlyVolume.term_ = FormulaResult::MONTHLY_VOLUME;
					auto value = parse_int_value(row[FormulaResult::MONTHLY_VOLUME]);
					monthlyVolume.op_ = value.first;
					monthlyVolume.value_ = static_cast<int64_t>(value.second);
					leaf.conditions_.emplace_back(monthlyVolume);
				}

				if (row.at(FormulaResult::FEE) != "")
				{
					auto fee = parse_fee(row[FormulaResult::FEE]);
					leaf.formula_ = fee;
				}

				//if (row.at(FormulaResult::UNDERLYING_SYMBOL) != "")
				//{
				//	condition underlyingSymbol;
				//	underlyingSymbol.term_ = to_string(FormulaResult::UNDERLYING_SYMBOL);
				//	underlyingSymbol.op_ = "=";
				//	underlyingSymbol.value_ = row[FormulaResult::UNDERLYING_SYMBOL];
				//	leaf.conditions_.emplace_back(underlyingSymbol);
				//}

				m_tree.branches_.emplace_back(leaf);
				i++;
			}
			catch (std::exception &ex)
			{
				std::stringstream sstrError;
				sstrError << " 'Variable of Tabular Plan is Missing' - " << ex.what();
				//LogWarning() << sstrError.str();
				throw std::exception(sstrError.str().c_str());
				return;
			}
			catch (...)
			{
				//LogWarning() << " 'Unable to Parse Tabular Method's Data'";
				throw std::exception("Unable to Parse Tabular Method's Data");
				return;
			}
		}
	}

	fee_formula TabularMethod::parse_fee(const std::string& fee)
	{
		try
		{
			if (boost::algorithm::contains(fee, "%"))
			{
				auto value = std::stod(fee.substr(0, fee.length() - 1));
				fee_per_value_formula fee_formula{ value };
				return fee_formula;
			}

			else if (boost::algorithm::contains(fee, "["))
			{
				auto value = std::stod(fee.substr(1, fee.length() - 1));
				fee_per_execution_formula fee_formula{ value };
				return fee_formula;
			}

			auto value = std::stod(fee);
			fee_per_share_formula fee_formula{ value };
			return fee_formula;
		}
		//catch (std::exception& ex)
		//{
		//	//LogWarning() << " 'Invalid Fee Received while Parsing:"<< ex.what() << "' " << fee;
		//}

		catch (...)
		{
			//LogWarning() << " 'Unable To Parse This Fee:' "<< fee;
		}
		fee_per_share_formula fee_formula{ 0 };
		return fee_formula;
	}

	void TabularMethod::check_members_existence(const rapidjson::Value& data)
	{
		std::vector<std::string> memberStrings = { "SequenceId",	"ExecBroker", "Contra", "Route",
												   "InternalRoute", "LIQ", "Tape", "Price",
												   "Quantity", "AfterHours", "Penny", "Time",
												   "InternalLiq", "BeforeHours","Lot","Fee", "Side", "MonthlyVolume"};
		for (const auto& member : memberStrings)
		{
			if (!data.HasMember(member.c_str()))
			{
				std::string temp = " 'Member of JSON Formula Doesn't Exist: "+ member;
				//LogWarning() << temp << " ' ";
				throw std::exception(temp.c_str());
			}
		}
	}

	void TabularMethod::parse_json(std::string& formula)
	{
		//std::cout << " 'Parsing JSON of Tabular Method'";
		rapidjson::Document document;
		
		if (formula.empty())
		{
			//LogWarning() << " 'JSON Method Is Empty Unable To Parse'";
			throw std::exception("JSON Method Is Empty Unable To Parse");
		}

		document.Parse(formula.c_str());

		if (document.HasParseError())
		{
			//LogWarning() << " 'Unable to Parse Tabular Formula'";
			throw std::exception("Unable to Parse Tabular Formula");
		}

		if (!document.IsArray())
		{
			//LogWarning() << " 'Unable to Parse Tabular Formula'";
			throw std::exception("Unable to Parse Tabular Formula");
		}

		std::string temp = "";
		for (rapidjson::Value::ConstValueIterator NoKeyInPair = document.Begin(); NoKeyInPair != document.End(); ++NoKeyInPair)
		{
			std::vector<std::string> formula;
			const rapidjson::Value& jsonData = *NoKeyInPair;
			
			check_members_existence(jsonData);

			/*0*/	formula.emplace_back(!jsonData["SequenceId"].IsNull() ? std::to_string(jsonData["SequenceId"].GetInt()) : "");
			/*1*/	formula.emplace_back(!jsonData["ExecBroker"].IsNull() ? jsonData["ExecBroker"].GetString() : "");
			/*2*/	formula.emplace_back(!jsonData["Contra"].IsNull() ? jsonData["Contra"].GetString() : "");
			/*3*/	formula.emplace_back(!jsonData["Route"].IsNull() ? jsonData["Route"].GetString() : "");
			/*4*/	formula.emplace_back(!jsonData["InternalRoute"].IsNull() ? jsonData["InternalRoute"].GetString() : "");
			/*5*/	formula.emplace_back(!jsonData["LIQ"].IsNull() ? jsonData["LIQ"].GetString() : "");
			/*6*/	formula.emplace_back(!jsonData["DST"].IsNull() ? jsonData["DST"].GetString() : "");
			/*7*/	formula.emplace_back(!jsonData["Tape"].IsNull() ? jsonData["Tape"].GetString() : "");
			/*8*/	formula.emplace_back(!jsonData["Price"].IsNull() ? jsonData["Price"].GetString() : "");
			/*9*/	formula.emplace_back(!jsonData["Quantity"].IsNull() ? jsonData["Quantity"].GetString() : "");
			/*10*/	formula.emplace_back(!jsonData["AfterHours"].IsNull() ? std::to_string(jsonData["AfterHours"].GetBool()) : "");
			/*11*/	formula.emplace_back(!jsonData["Penny"].IsNull() ? std::to_string(jsonData["Penny"].GetBool()) : "");
			/*12*/	formula.emplace_back(!jsonData["Time"].IsNull() ? jsonData["Time"].GetString() : "");
			/*13*/	formula.emplace_back(!jsonData["Type"].IsNull() ? std::to_string(jsonData["Type"].GetInt()) : "");
			/*14*/	formula.emplace_back(!jsonData["InternalLiq"].IsNull() ? jsonData["InternalLiq"].GetString() : "");
			/*15*/	formula.emplace_back(!jsonData["BeforeHours"].IsNull() ? std::to_string(jsonData["BeforeHours"].GetBool()) : "");
			/*16*/	formula.emplace_back(!jsonData["Lot"].IsNull() ? jsonData["Lot"].GetString() : "");
			/*17*/	formula.emplace_back(!jsonData["Fee"].IsNull() ? jsonData["Fee"].GetString() : "");
			/*18*/	formula.emplace_back(!jsonData["Side"].IsNull() ? std::to_string(jsonData["Side"].GetInt()) : "");
			/*19*/	formula.emplace_back(!jsonData["MonthlyVolume"].IsNull() ? jsonData["MonthlyVolume"].GetString() : "");

			parsed_formulas.emplace_back(formula);
		}
		sort(parsed_formulas.begin(), parsed_formulas.end(), 
		[](const std::vector <std::string>& lhs, const std::vector <std::string>& rhs) 
		{ 
			try
			{
				int a = std::stoi(lhs[0]);
				int b = std::stoi(rhs[0]);
				return (a < b);
			}
			catch(...)
			{
				return false;
			}
		});
	}

	bool TabularMethod::string_to_bool(const std::string& str)
	{
		std::string check = boost::algorithm::to_lower_copy(str);
		if (check == "true" || str == "1")
			return true;
		else
			return false;
	}

	value TabularMethod::parse_string_values(const std::string& str)
	{
		std::vector<std::string> parsed;
		boost::split(parsed, str, boost::is_any_of(","));
		if (parsed.size() == 1)
		{
			return parsed.at(0);
		}
		else
		{
			std::unordered_set<std::string> setContainer(parsed.begin(), parsed.end());
			return setContainer;
		}
	}

	result TabularMethod::evaluate(const OrderExecutionData& exec)
	{
		//std::cout << " 'Evaluating Fee using Tabular Method'";
		return m_tree.evaluate(exec);
	}

	template <>
	bool ge_evaluator::operator()(const double& lval, const std::int64_t& rval) const
	{
		return gt_evaluator()(lval, rval) || eq_evaluator()(lval, rval);
	}

	template <>
	bool ge_evaluator::operator()(const std::int64_t& lval, const double& rval) const
	{
		return gt_evaluator()(lval, rval) || eq_evaluator()(lval, rval);
	}

	template <>
	bool gt_evaluator::operator()(const double& lval, const std::int64_t& rval) const
	{
		return (lval > (double)rval);
	}

	template <>
	bool gt_evaluator::operator()(const std::int64_t& lval, const double& rval) const
	{
		return ((double)lval > rval);
	}

	template <>
	bool eq_evaluator::operator()(const std::string& lval, const std::unordered_set<std::string>& rval) const
	{
		return rval.count(lval) > 0 ? true : false;
		//return (std::find(rval.begin(), rval.end(), lval) != rval.end());
	}

	template <>
	bool eq_evaluator::operator()(const double& lval, const std::int64_t& rval) const
	{
		return fabs(lval - (double)rval) < std::numeric_limits<double>::epsilon();
	}

	template <>
	bool eq_evaluator::operator()(const std::int64_t& lval, const double& rval) const
	{
		return fabs((double)lval - rval) < std::numeric_limits<double>::epsilon();
	}

	template <>
	bool lt_evaluator::operator()(const double& lval, const std::int64_t& rval) const
	{
		return (lval < (double)rval);
	}

	template <>
	bool lt_evaluator::operator()(const std::int64_t& lval, const double& rval) const
	{
		return ((double)lval < rval);
	}

	template <>
	bool le_evaluator::operator()(const double& lval, const std::int64_t& rval) const
	{
		return lt_evaluator()(lval, rval) || eq_evaluator()(lval, rval);
	}

	template <>
	bool le_evaluator::operator()(const std::int64_t& lval, const double& rval) const
	{
		return lt_evaluator()(lval, rval) || eq_evaluator()(lval, rval);
	}

	double evaluate_fee(const fee_formula& formula, const OrderExecutionData& exec)
	{
		fee_evaluator eval(exec);
		return std::visit(eval, formula);
		//return boost::apply_visitor(eval, formula);
	}

	result evaluate_rule(const rule& rule, const OrderExecutionData& exec)
	{
		return rule.evaluate(exec);
		//rule_evaluator eval(exec);
		//return boost::apply_visitor(eval, rule);
	}
}