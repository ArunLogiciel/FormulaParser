#pragma once
#include <typeinfo>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "rapidjson/document.h"
#include "Interfaces/MethodInterface.h"
//#include "logger/logger.hpp"
#include "businessObjects.h"
#include <unordered_set>



namespace FeeFormula 
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	using string_or_set = std::variant<std::string, std::unordered_set<std::string>>;
	using value = std::variant<bool, std::int64_t, double, std::string, std::unordered_set<std::string>>;//,string_or_set>;

	// <editor-fold desc="execution">
	//These Variables are Being Recognized Only

	//static const std::string& to_string(formula_result s) 
	//{
	//	static const std::string FORMULA_ID			=	"formulaId";
	//	static const std::string EXECBROKER			=	"execBroker";
	//	static const std::string CONTRA				=	"contra";
	//	static const std::string ROUTE				=	"route";
	//	static const std::string INTERNAL_ROUTE		=	"internalRoute";
	//	static const std::string LIQ				=	"liq";
	//	static const std::string DST				=	"dst";
	//	static const std::string TAPE				=	"tape";
	//	static const std::string PRICE				=	"price";
	//	static const std::string QUANTITY			=	"quantity";
	//	static const std::string AFTERHOURS			=	"afterHours";
	//	static const std::string PENNY				=	"penny";
	//	static const std::string TIME				=	"time";
	//	static const std::string TYPE				=	"type";
	//	static const std::string INTERNAL_LIQ		=	"internalLiq";
	//	static const std::string BEFOREHOURS		=	"beforeHours";
	//	static const std::string LOT				=	"lot";
	//	static const std::string FEE				=	"fee";
	//	static const std::string SIDE				=	"side";
	//	static const std::string MONTHLY_VOLUME		=	"monthlyVolume";
	//	static const std::string UNDERLYING_SYMBOL	=	"underlyingSymbol";
	//	static const std::string UNKNOWN			=	"unknown";

	//	switch (s) 
	//	{
	//		case formula_result::FORMULA_ID:
	//			return FORMULA_ID;
	//		case formula_result::EXECBROKER:
	//			return EXECBROKER;
	//		case formula_result::CONTRA:
	//			return CONTRA;
	//		case formula_result::ROUTE:
	//			return ROUTE;
	//		case formula_result::INTERNAL_ROUTE:
	//			return INTERNAL_ROUTE;
	//		case formula_result::LIQ:
	//			return LIQ;
	//		case formula_result::DST:
	//			return DST;
	//		case formula_result::TAPE:
	//			return TAPE;
	//		case formula_result::PRICE:
	//			return PRICE;
	//		case formula_result::QUANTITY:
	//			return QUANTITY;
	//		case formula_result::AFTERHOURS:
	//			return AFTERHOURS;
	//		case formula_result::PENNY:
	//			return PENNY;
	//		case formula_result::TIME:
	//			return TIME;
	//		case formula_result::TYPE:
	//			return TYPE;
	//		case formula_result::INTERNAL_LIQ:
	//			return INTERNAL_LIQ;
	//		case formula_result::BEFOREHOURS:
	//			return BEFOREHOURS;
	//		case formula_result::LOT:
	//			return LOT;
	//		case formula_result::FEE:
	//			return FEE;
	//		case formula_result::SIDE:
	//			return SIDE;
	//		case formula_result::MONTHLY_VOLUME:
	//			return MONTHLY_VOLUME;		
	//		case formula_result::UNDERLYING_SYMBOL:
	//			return UNDERLYING_SYMBOL;
	//		default:
	//			return UNKNOWN;
	//	}
	//}

	struct eq_evaluator : public boost::static_visitor<bool> 
	{
		template <typename T, typename U>
		bool operator()(const T& t, const U& u) const 
		{
			std::cout << "Type of T: " << typeid(t).name() << std::endl;
			std::cout << "Type of U: " << typeid(u).name() << std::endl;
			return false; // cannot compare different types
		}

		template <typename T>
		bool operator()(const T& lhs, const T& rhs) const 
		{
			return lhs == rhs;
		}

		template <>
		bool operator()(const std::string& lval, const std::unordered_set<std::string>& rval) const;

		template <>
		bool operator()(const double& lval, const std::int64_t& rval) const;

		template <>
		bool operator()(const std::int64_t& lval, const double& rval) const;
	};

	struct lt_evaluator : public boost::static_visitor<bool> 
	{
		template <typename T, typename U>
		bool operator()(const T&, const U&) const 
		{
			return false; // cannot compare different types
		}

		template <typename T>
		bool operator()(const T& lhs, const T& rhs) const 
		{
			return lhs < rhs;
		}

		template <>
		bool operator()(const double& lval, const std::int64_t& rval) const;

		template <>
		bool operator()(const std::int64_t& lval, const double& rval) const;
	};

	struct le_evaluator : public boost::static_visitor<bool> 
	{
		template <typename T, typename U>
		bool operator()(const T&, const U&) const 
		{
			return false; // cannot compare different types
		}

		template <typename T>
		bool operator()(const T& lhs, const T& rhs) const 
		{
			return lhs <= rhs;
		}

		template <>
		bool operator()(const double& lval, const std::int64_t& rval) const;

		template <>
		bool operator()(const std::int64_t& lval, const double& rval) const;
	};

	struct gt_evaluator : public boost::static_visitor<bool> 
	{
		template <typename T, typename U>
		bool operator()(const T&, const U&) const 
		{
			return false; // cannot compare different types
		}

		template <typename T>
		bool operator()(const T& lhs, const T& rhs) const 
		{
			return lhs > rhs;
		}

		template <>
		bool operator()(const double& lval, const std::int64_t& rval) const;

		template <>
		bool operator()(const std::int64_t& lval, const double& rval) const;
	};

	struct ge_evaluator : public boost::static_visitor<bool> 
	{
		template <typename T, typename U>
		bool operator()(const T&, const U&) const 
		{
			return false; // cannot compare different types
		}

		template <typename T>
		bool operator()(const T& lhs, const T& rhs) const 
		{
			return lhs >= rhs;
		}
		template<>
		bool operator()(const double& lval, const std::int64_t& rval) const;

		template <>
		bool operator()(const std::int64_t& lval, const double& rval) const;
	};

	struct condition 
	{
		[[nodiscard]] bool evaluate(const OrderExecutionData& exec) const 
		{
			if (op_ == EN_EqualityOperator_EqualsTo)
			{
				return std::visit(eq_evaluator(), exec.get_attribute(term_), value_);
				//return boost::apply_visitor(eq_evaluator(), exec.get_attribute(term_), value_);
			}

			else if (op_ == EN_EqualityOperator_NotEqualsTo)
			{
				return !std::visit(eq_evaluator(), exec.get_attribute(term_), value_);
				//return !boost::apply_visitor(eq_evaluator(), exec.get_attribute(term_), value_);
			}

			else if (op_ == EN_EqualityOperator_LessThan)
			{
				return std::visit(lt_evaluator(), exec.get_attribute(term_), value_);
				//return boost::apply_visitor(lt_evaluator(), exec.get_attribute(term_), value_);
			}

			else if (op_ == EN_EqualityOperator_LessThanEqualsTo)
			{
				return std::visit(le_evaluator(), exec.get_attribute(term_), value_);
				//return boost::apply_visitor(le_evaluator(), exec.get_attribute(term_), value_);
			}

			else if (op_ == EN_EqualityOperator_GreaterThan)
			{
				return std::visit(gt_evaluator(), exec.get_attribute(term_), value_);
				//return boost::apply_visitor(gt_evaluator(), exec.get_attribute(term_), value_);
			}

			else if (op_ == EN_EqualityOperator_GreaterThanEqualsTo)
			{
				return std::visit(ge_evaluator(), exec.get_attribute(term_), value_);
				//return boost::apply_visitor(ge_evaluator(), exec.get_attribute(term_), value_);
			}

			else 
			{
				throw std::runtime_error("unsupported condition operation: '" + std::to_string(op_) + "'");
			}
		}

		void printVariant(const OrderExecutionData& exec) const
		{
			/*
			if (value_.type() == typeid(std::vector<std::string>))
			{
				std::cout << "VectorExpected = " << boost::get<std::vector<std::string>>(value_)[0];
				std::cout << " Vs VectorReceived = " << boost::get<std::vector<std::string>>(exec.get_attribute(term_))[0] << std::endl;
			}

			if (value_.type() == typeid(std::string))
			{
				std::cout << "StringExpected = " << boost::get<std::string>(value_);
				std::cout << " Vs StringReceived = " << boost::get<std::string>(exec.get_attribute(term_)) << std::endl;
			}

			if (value_.type() == typeid(bool))
			{
				std::cout << "BoolExpected = " << boost::get<bool>(value_);
				std::cout << " Vs BoolReceived = " << boost::get<bool>(exec.get_attribute(term_)) << std::endl;
			}

			if (value_.type() == typeid(double))
			{
				std::cout << "DoubleExpected = " << boost::get<double>(value_);
				std::cout << " Vs DoubleReceived = " << boost::get<double>(exec.get_attribute(term_)) << std::endl;
			}

			if (value_.type() == typeid(int64_t))
			{
				std::cout << "int64Expected = " << boost::get<int64_t>(value_);
				std::cout << " Vs int64Received = " << boost::get<int64_t>(exec.get_attribute(term_)) << std::endl;
			}
			*/
		}
		FormulaResult term_;
		EqualityOperator op_;
		value value_;
	};

	using condition_chain = std::vector<condition>;
	struct fee_per_share_formula 
	{
		[[nodiscard]] double evaluate(const OrderExecutionData& exec) const 
		{
			return fee_per_share_ * exec.get_qty();
		}
		double fee_per_share_;
	};

	struct fee_per_value_formula 
	{
		[[nodiscard]] double evaluate(const OrderExecutionData& exec) const 
		{
			return fee_per_value_ * (exec.get_qty() * exec.get_price());
		}
		double fee_per_value_;
	};

	struct fee_per_execution_formula 
	{
		[[nodiscard]] double evaluate(const OrderExecutionData& exec) const 
		{
			return fee_per_execution_;
		}
		double fee_per_execution_;
	};

	using fee_formula = std::variant<fee_per_share_formula, fee_per_value_formula, fee_per_execution_formula>;
	struct fee_evaluator : public boost::static_visitor<double> 
	{
		explicit fee_evaluator(const OrderExecutionData& exec) : exec_(exec) {}

		template <typename FeeFormula>
		[[nodiscard]] double operator()(const FeeFormula& formula) const 
		{
			return formula.evaluate(exec_);
		}

		const OrderExecutionData& exec_;
	};


	[[nodiscard]] double evaluate_fee(const fee_formula& formula, const OrderExecutionData& exec);

	//using fee_rule_collection = boost::container::small_vector<fee_formula, 3>;
	using result = std::pair<bool, double>;

	struct leaf_rule 
	{
		[[nodiscard]] result evaluate(const OrderExecutionData& exec) const 
		{
			// check all conditions
			for (const auto& cond : conditions_) 
			{
				if (!cond.evaluate(exec)) 
				{
					return { false, 0.0 };	//if condition fails
				}
			}

			// all conditions passed -> return the leaf value
			return { true, evaluate_fee(formula_, exec) };
		}
		condition_chain conditions_;
		fee_formula formula_;
	};

	//struct branch_rule;
	//using rule = boost::variant<leaf_rule, boost::recursive_wrapper<branch_rule>>;
	using rule = leaf_rule;

	//struct rule_evaluator : public boost::static_visitor<result> 
	//{
	//	explicit rule_evaluator(const OrderExecutionData& exec) : exec_(exec) {}

	//	template <typename RuleType>
	//	[[nodiscard]] result operator()(const RuleType& rule) const 
	//	{
	//		return rule.evaluate(exec_);
	//	}

	//	const OrderExecutionData& exec_;
	//};

	[[nodiscard]] result evaluate_rule(const rule& rule, const OrderExecutionData& exec);

	//struct branch_rule 
	//{
	//	[[nodiscard]] result evaluate(const OrderExecutionData& exec) const 
	//	{
	//		// check all conditions
	//		for (const auto& cond : conditions_) 
	//		{
	//			if (!cond.evaluate(exec)) 
	//			{
	//				return { false, 0.0 };
	//			}
	//		}

	//		// all conditions passed -> search for a child rule that evaluates true
	//		for (const auto& child_rule : children_) 
	//		{
	//			result res = evaluate_rule(child_rule, exec);
	//			if (res.first) 
	//			{
	//				return res;
	//			}
	//		}

	//		return { false, 0.0 };
	//	}

	//	condition_chain conditions_;
	//	std::vector<rule> children_;
	//};

	struct tree 
	{
		[[nodiscard]] result evaluate(const OrderExecutionData& exec) const 
		{
			int i = 0; //SequenceId
			for (const auto& branch : branches_) 
			{
				result res = evaluate_rule(branch, exec);
				if (res.first) 
				{
					//std::cout << __PRETTY_FUNCTION__ << " 'Plan Sequence Executed: " << i << "'";
					return res;
				}
				i++;
			}
			//std::cout << __PRETTY_FUNCTION__ << " 'No Plan Sequence Executed Returning Default Fee: 0.0'";
			return { false, 0.0 };
		}
		std::vector<rule> branches_;
	};

	class TabularMethod : public LSL::FeeModule::Interfaces::MethodInterface //This Class is basically a wrapper of Above Tree & Rule Functions
	{
		private:
			std::vector <std::vector <std::string>>  parsed_formulas;
			tree m_tree;	//Tree is the Main DataStructure Here
			[[nodiscard]] inline bool string_to_bool(const std::string& str);
			[[nodiscard]] inline value parse_string_values(const std::string&);
			[[nodiscard]] inline std::pair<EqualityOperator, double> parse_double_value(const std::string&);
			[[nodiscard]] inline std::pair<EqualityOperator, int64_t> parse_int_value(const std::string&);
			[[nodiscard]] inline std::pair<EqualityOperator, std::string> parse_string_value(const std::string& str);
			[[nodiscard]] inline fee_formula parse_fee(const std::string& str);
			void make_tree(std::vector<std::vector<std::string>>& p_table);
			void parse_json(std::string& formula) final override;
			void check_members_existence(const rapidjson::Value& data);
			TabularMethod() = delete;
		
		public:
			TabularMethod(std::string& formula);						//Calls Tree->make_tree()
			result evaluate(const OrderExecutionData&) final override;	//Calls Tree->evaluate()	
	};
}
