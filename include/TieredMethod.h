#pragma once
#include "Interfaces/MethodInterface.h"
#include "rapidjson/document.h"

class TieredMethod : public LSL::FeeModule::Interfaces::MethodInterface
{
private:
	TierPlan tier_plan;
	void parse_json(std::string& formula) final override;
	std::pair<bool, double> calculateTieredFee(const std::vector<Tier>& tiers, double checkQty, double quantity);
	std::pair<bool, double> calculateTieredFee(const std::vector<Tier>& tiers, double qty);
	std::pair<bool, double> calculateTieredFeeRegressive(const std::vector<Tier>& tiers, double prevQty, double deltaQty);
	void check_members_existence(const rapidjson::Value& data);
	TieredMethod() = delete;

public:
	TieredMethod(std::string formulaJSON);
	std::pair<bool, double> evaluate(const OrderExecutionData&) final override;
};