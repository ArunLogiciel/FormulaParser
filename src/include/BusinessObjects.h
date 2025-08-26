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

#pragma once
