#if defined(__GNUC__) || defined(__clang__)
#define MY_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define MY_PRETTY_FUNCTION __FUNCSIG__
#else
#define MY_PRETTY_FUNCTION "Unknown Function"
#endif


#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <optional>
#include <format>
enum FormulaResult
{
	FORMULA_ID,
	EXECBROKER,
	CONTRA,
	ROUTE,
	INTERNAL_ROUTE,
	LIQ,
	DST,
	TAPE,
	PRICE,
	QUANTITY,
	AFTERHOURS,
	PENNY,
	TIME,
	TYPE,
	INTERNAL_LIQ,
	BEFOREHOURS,
	LOT,
	FEE,
	SIDE,
	MONTHLY_VOLUME,
	UNDERLYING_SYMBOL
};
enum FormulaType
{
	EN_FormulaType_None = 0,
	EN_FormulaType_Tabular = 1,
	EN_FormulaType_Tiered = 2,
	EN_FormulaType_Raw = 3,
	EN_FormulaType_Builder = 4,
	EN_FormulaType_AdvBuilder = 5,
	EN_FormulaType_Coded = 6
};
enum  EqualityOperator
{
	EN_EqualityOperator_None = 0,
	EN_EqualityOperator_EqualsTo = 1,
	EN_EqualityOperator_LessThanEqualsTo = 2,
	EN_EqualityOperator_GreaterThanEqualsTo = 3,
	EN_EqualityOperator_LessThan = 4,
	EN_EqualityOperator_GreaterThan = 5,
	EN_EqualityOperator_NotEqualsTo = 6
};

using Fee = double;

struct FeeInfo {
	Fee orderFee{};
	Fee executionFee{};

	FeeInfo() : orderFee(0), executionFee(0) {}

	friend std::ostream& operator<<(std::ostream& os, const FeeInfo& data) {
		os << "OrderFee:" << data.orderFee
			<< " - ExecutionFee:" << data.executionFee << " |";
		return os;
	}

	std::string getString() const {
		std::ostringstream oss;
		oss << "OrderFee:" << orderFee << " | ExecutionFee:" << executionFee << " |";
		return oss.str();
	}
};

struct FeeBreakDown {
	Fee orderFee{};
	Fee executionFee{};
	std::string feeCategoryId{};
	std::string planId{};
	std::string logs{};

	FeeBreakDown() : orderFee(0), executionFee(0), planId(""), feeCategoryId(""), logs("") {}

	friend std::ostream& operator<<(std::ostream& os, const FeeBreakDown& data) {
		os << "OrderFee:" << data.orderFee
			<< " - ExecutionFee:" << data.executionFee
			<< " - PlanId:" << data.planId
			<< " - FeeCategoryId:" << data.feeCategoryId << " |";
		return os;
	}

	std::string getString() const {
		std::ostringstream oss;
		oss << "OrderFee:" << orderFee
			<< " | ExecutionFee:" << executionFee
			<< " | PlanId:" << planId
			<< " | FeeCategoryId:" << feeCategoryId;
		return oss.str();
	}
};

struct FeeComissionData {
	FeeInfo totalTradeFee;
	FeeInfo comission;
	std::vector<FeeBreakDown> tradeFeeBreakDown;

	FeeComissionData() : totalTradeFee(), comission() {}

	friend std::ostream& operator<<(std::ostream& os, const FeeComissionData& data) {
		os << "TotalTradeFee | " << data.totalTradeFee
			<< "Commission:" << data.comission << " |";

		for (auto& fee : data.tradeFeeBreakDown) {
			os << fee;
		}
		return os;
	}

	std::string getStringFeeCommission() const {
		std::ostringstream oss;
		oss << "TotalTradeFee:" << totalTradeFee.getString()
			<< " | Commission:" << comission.getString() << " |";
		std::string s = oss.str();
		for (auto& fee : tradeFeeBreakDown) {
			s += fee.getString();
		}
		return s;
	}
};
struct ExecutionResult {
	std::string executionId;
	double totalFee = 0.0;
};
// Aliases
struct OrderExecutionData
{
	using value = std::variant <bool, std::int64_t, double, std::string, std::vector<std::string>>;
	[[nodiscard]] value get_attribute(const FormulaResult& key) const
	{
		if (key == ROUTE)
		{
			return route;
		}
		else if (key == PENNY)
		{
			return penny;
		}
		else if (key == LIQ)
		{
			return liq;
		}
		else if (key == EXECBROKER)
		{
			return execBroker;
		}
		else if (key == INTERNAL_ROUTE)
		{
			return internalRoute;
		}
		else if (key == PRICE)
		{
			return price;
		}
		else if (key == CONTRA)
		{
			return contra;
		}
		else if (key == DST)
		{
			return dst;
		}
		else if (key == TAPE)
		{
			return tape;
		}
		else if (key == MONTHLY_VOLUME)
		{
			return monthlyVolume;
		}
		else if (key == TYPE)
		{
			return static_cast<int64_t>(type);
		}
		else if (key == AFTERHOURS)
		{
			return afterHours;
		}
		else if (key == INTERNAL_LIQ)
		{
			return internalLiq;
		}
		else if (key == BEFOREHOURS)
		{
			return beforeHours;
		}
		else if (key == LOT)
		{
			switch (lot)
			{
			case 1:
				return "ODD";
			case 2:
				return "EVEN";
			case 3:
				return "MIXED";
			default:
				return "";
			}
		}
		else if (key == TIME)
		{
			return time;
		}
		else if (key == QUANTITY)
		{
			return quantity;
		}
		else if (key == SIDE)
		{
			return static_cast<int64_t>(side);
		}
		else if (key == UNDERLYING_SYMBOL)
		{
			return underlyingSymbol;
		}
		else
		{
			throw std::runtime_error("bad key: '" + std::to_string(key) + "'");
		}
	}

	[[nodiscard]] double get_price() const { return price; }
	[[nodiscard]] int64_t get_qty() const { return quantity; }
	std::string route{ "" };
	std::string liq{ "" };
	std::string execBroker{ "" };
	std::string contra{ "" };
	std::string dst{ "" };
	std::string internalLiq{ "" };
	std::string internalRoute{ "" };
	std::string time{ "" };
	std::string date{ "" };
	std::string symbol{ "" };
	std::string currency{ "" };

	double price{ 0.0 };
	double avgPx{ 0.00 };
	double lastPx{ 0.00 };

	int64_t quantity{ 0 };
	int64_t monthlyVolume{ 0 };
	int64_t fillCount{ 0 };
	int64_t execQty{ 0 };
	int64_t lastShares{ 0 };

	bool afterHours{ false };
	bool beforeHours{ false };
	bool penny{ false };

	int lot{};
	int side{};
	int capacity{};

	//DataId & ForeignKeys
	std::string orderId{ "" };
	std::string executionId{ "" };
	std::string accountId{ "" };
	std::string accountValue{ "" };
	std::string firmId{ "" };
	std::string mpidId{ "" };
	std::string fileName{ "" };
	std::string dfidRecv{ "" };
	std::string mpidRecv{ "" };
	std::string uniqueId{ "" };
	std::string ExchangeType{ "" };

	//TO DO: Add Support For These | From FEE RULES documentation of PropReports
	double mult{ 0 };						//The multiplier(sometimes called "contract size" for options and "value multiplier" for futures) associated with the traded instrument and used when computing profit and loss
	int type{};					//Type of instrument being traded (equity, option, future, index, fund, fx, bond)
	std::string subType{ "" };				//For equities, this will be either etf or blank
	std::string underlyingSymbol{ "" };		//For options, the ticker symbol of the underlying security or index.
	std::string tape{ "" };					//Trade reporting facility indicator (e.g. A, B, or C). 	
	std::string underlyingType{ "" };		//For options, the type of the underlying security (equity, future, index)


	friend std::ostream& operator << (std::ostream& os, const OrderExecutionData& data)
	{
		os << "OrderID:" << data.orderId << '|' << "ExecutionID:" << data.executionId << '|' << "Account:" << data.accountId << '|' << '|' << "Route:" << data.route << '|' << "Penny:" << data.penny << '|'
			<< "LIQ:" << data.liq << '|' << "Price:" << data.price << '|' << "Quantity:" << data.quantity << '|' << "ExecBroker:" << data.execBroker << '|' << "Contra:" << data.contra << '|'
			<< "DST:" << data.dst << '|' << "Tape:" << data.tape << '|' << "MonthlyVolume:" << data.monthlyVolume << '|' << "Type:" << data.type << '|' << "BeforeHours:" << data.beforeHours << '|'
			<< "AfterHours:" << data.afterHours << '|' << "InternalLIQ:" << data.internalLiq << '|' << "InternalRoute:" << data.internalRoute << '|' << "Lot:" << data.lot << '|' << "Time:" << data.time << '|' << "Date:" << data.date << '|'
			<< "Side:" << data.side << '|' << "Symbol:" << data.symbol << '|' << "ExecQty:" << data.execQty << '|' << "AvgPx:" << data.avgPx << '|' << "LastShares:" << data.lastShares << '|'
			<< "LastPx:" << data.lastPx << '|' << "Capacity:" << data.capacity << '|' << "FillCount:" << data.fillCount << '|' << "Currency:" << data.currency << '|' << "FirmID:" << data.firmId << '|';
		return os;
	}
	bool operator < (const OrderExecutionData& other) const
	{
		// Compare based on field1, field2, and field3
		return std::tie(other.accountValue, other.time, other.orderId, other.executionId) <
			std::tie(accountValue, time, orderId, executionId);
	}
};


struct IdentifiableVariables
{
	double Type = 0;		//Enum
	double Penny = 0;		//Bool
	double BeforeHours = 0; //Bool
	double AfterHours = 0;	//Bool
	double Price = 0;
	double Quantity = 0;
	double MonthlyVolume = 0;
	double Fees = 0;

	std::string Route{ "" };
	std::string LIQ{ "" };
	std::string LOT{ "" };
	std::string DST{ "" };
	std::string ExecBroker{ "" };
	std::string Contra{ "" };
	std::string Tape{ "" };
	std::string Time{ "" };
	std::string InternalLiq{ "" };
	std::string InternalRoute{ "" };
	std::string Side{ "" };
	std::string UnderlyingSymbol{ "" }; //Adding new to support plans
	std::string ExchangeType{ "" };

	void copy(const OrderExecutionData& rhs)
	{
		Penny = bool_to_double(rhs.penny);
		AfterHours = bool_to_double(rhs.afterHours);
		BeforeHours = bool_to_double(rhs.beforeHours);
		Price = static_cast <double> (rhs.price);
		Quantity = static_cast<double>(rhs.quantity);
		MonthlyVolume = static_cast <double> (rhs.monthlyVolume);
		Route = rhs.route;
		LIQ = rhs.liq;
		DST = rhs.dst;
		LOT = rhs.lot;
		Contra = rhs.contra;
		Tape = rhs.tape;
		Time = rhs.time;
		Type = static_cast<int>(rhs.type);
		InternalLiq = rhs.internalLiq;
		InternalRoute = rhs.internalRoute;
		ExecBroker = rhs.execBroker;
		UnderlyingSymbol = rhs.underlyingSymbol;
		char temp = static_cast<int>(rhs.side); //To Make Side Compatible with ExprTk
		Side = temp;							//This is Necessary
		ExchangeType = rhs.ExchangeType;
	}

	double bool_to_double(bool check)
	{
		return (check ? 1 : 0);
	}
};
struct ConditionFormula {
	int sequenceId = 0;
	std::string execBroker;
	std::string contra;
	std::string route;
	std::string internalRoute;
	std::string liq;
	std::string dst;
	std::string tape;

	std::optional<double> price;
	std::optional<long long> quantity;
	std::optional<bool> beforeHours;
	std::optional<bool> afterHours;
	std::optional<bool> penny;

	std::string time;
	std::optional<int> type;
	std::optional<int> side;
	std::string internalLiq;
	std::string lot;
	std::optional<long long> monthlyVolume;

	std::string fee;
};

struct TieredTableEntry {
	std::optional<double> startRange;
	std::optional<double> endRange;
	std::string fee;
	int sequenceId;
};

struct FormulaData {
	int tieredType = 0;
	std::vector<TieredTableEntry> tieredTable;
	bool isRegressive = false;
	std::vector<ConditionFormula> conditions;

};

struct PlanData {
	std::string id;
	std::string name;
	int rounding = 0;
	int roundingPoints = 0;
	int planType = 0;
	std::string firmId;
	std::string created;
	std::string createdBy;
	std::string modified;
	std::string modifiedBy;
	std::string formula_final;
	bool isDeleted = false;
	bool isSystemGenerated = false;
	bool isActive = false;
	std::string feeCategoryId;
	int formulaType = 0;
	FormulaData formula;
	bool isCompiled = false;
	int feeCategoryType = 0;
	int incrementalPlanId = 0;
	std::optional<int> parentPlanId;
};
enum TierType
{
	EN_Tier_None = 0,
	EN_Tier_Price = 1,
	EN_Tier_Quantity = 2,
	EN_Tier_Monthly_Volume = 3
};

struct Tier
{
	double sequence{};
	//double start{};	//MR: Not Required Anymore
	double end{};
	double fee{};
};
struct TierPlan
{
	std::string id{};
	TierType type{};
	std::vector<Tier> tiers{};
	bool isRegressive{ false };	//MR: Let's use Initializer List as per the above convention
};
#pragma once
