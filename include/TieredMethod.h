#pragma once
#include "Interfaces/MethodInterface.h"
#include "rapidjson/document.h"

class TieredMethod : public LSL::FeeModule::Interfaces::MethodInterface
{
	private:
		TierPlan tier_plan;
		void parse_json(std::string& formula) final override;
		template<typename T>
		std::pair<bool, double> calculateTieredFee(std::vector<Tier>& Tiers, T checkQty, int64_t quantity);
		std::pair<bool, double> calculateTieredFeeRegressive(std::vector<Tier>& Tiers, int64_t checkQty, int64_t quantity);
		std::pair<bool, double> calculateTieredFeeNonRegressive(std::vector<Tier>& Tiers, int64_t checkQty, int64_t quantity);
		void check_members_existence(const rapidjson::Value& data);
		TieredMethod() = delete;
		
	public:
		TieredMethod(std::string formulaJSON);
		std::pair<bool,double> evaluate(const OrderExecutionData&) final override;
};
