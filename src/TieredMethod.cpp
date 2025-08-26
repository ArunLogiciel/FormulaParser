#pragma once
#include "TieredMethod.h"
#include "rapidjson/document.h"
#include "ParsingException.h"
TieredMethod::TieredMethod(std::string formulaJSON)
{
    parse_json(formulaJSON);
}

std::pair<bool, double> TieredMethod::evaluate(const OrderExecutionData& data)
{
    std::pair<bool,double> fee = {};

    switch (tier_plan.type)
    {
        case EN_Tier_Monthly_Volume:
            
            if (tier_plan.isRegressive)
            {
                fee = calculateTieredFeeRegressive(tier_plan.tiers, data.monthlyVolume, data.quantity);
            }
            else
            {
                fee = calculateTieredFee(tier_plan.tiers, data.monthlyVolume, data.quantity);
            }
            break;

        case EN_Tier_Price:
            fee = calculateTieredFee(tier_plan.tiers, data.price, data.quantity);
            break;

        case EN_Tier_Quantity:
            fee = calculateTieredFee(tier_plan.tiers, data.quantity, data.quantity);
            break;
    }
    return fee;
}

void TieredMethod::parse_json(std::string& formula)
{

    
    if (formula.empty())
    {
        throw ParsingException("JSON Method Is Empty Unable To Parse");
    }

    rapidjson::Document document;
    document.Parse(formula.c_str());

    if (document.HasParseError())
    {
        throw ParsingException("Failed To Parse Tiered Formula");
    };

    if (!(document.HasMember("TieredType")) || !(document.HasMember("TieredTable")) || !(document.HasMember("isRegressive")) )    //MR: Added Check for new Bool
    {
        throw ParsingException("Failed To Parse Tiered Formula");
    }

    tier_plan.type = static_cast<TierType>(document["TieredType"].GetInt());
    tier_plan.isRegressive = document["isRegressive"].GetBool();// MR: Added To Get Value of bool from JSON
    const rapidjson::Value& json_tiers = document["TieredTable"];
    for (rapidjson::Value::ConstValueIterator value = json_tiers.Begin(); value != json_tiers.End(); ++value)
    {

        const rapidjson::Value& jsonData = *value;
        Tier tier;
        check_members_existence(jsonData);
        tier.sequence = jsonData["SequenceId"].IsNull() ? 0          :  jsonData["SequenceId"].GetInt(); 
        //tier.start    = jsonData["StartRange"].IsNull() ? 0          :  jsonData["StartRange"].GetDouble();   MR: Not Required Anymore
        tier.end      = jsonData["EndRange"].IsNull()   ? INT64_MAX  :  jsonData["EndRange"].GetDouble();
        tier.fee      = jsonData["Fee"].IsNull()        ? 0.0        :  std::stod(jsonData["Fee"].GetString());

        tier_plan.tiers.push_back(tier);
    }
}

void TieredMethod::check_members_existence(const rapidjson::Value& data)
{
    std::vector<std::string> memberStrings = { "SequenceId", "EndRange", "Fee" };    //Removed Check for Removed Member
    for (const auto& member : memberStrings)
    {
        if (!data.HasMember(member.c_str()))
        {
            throw ParsingException("Some Member of JSON Formula Doesn't Exist");
        }
    }
}

std::pair<bool, double> TieredMethod::calculateTieredFeeRegressive(std::vector<Tier>& Tiers, int64_t checkQty, int64_t quantity) 
{
    double fee = 0.0;
    bool tierCrossed = false;
    double rebate = 0;
    int newTierIndex = 0;

    //Get Current Tier
    for (int i = 0; i < Tiers.size() - 1; ++i) 
    {   
        //excluding default end tier
        if (checkQty - quantity > Tiers[i].end) 
        { //If Old & New Qty are both Next Tier
            tierCrossed = false;
            continue;
        }

        else if (checkQty - quantity <= Tiers[i].end && (checkQty > Tiers[i].end)) 
        {
            //If Tiered Crossed
            tierCrossed = true;
            rebate = Tiers[i].end * (Tiers[i].fee - Tiers[i + 1].fee);
            newTierIndex = i + 1;
            break;
        }

        else if (checkQty <= Tiers[i].end) 
        { 
            //If totally in this tier
            tierCrossed = false;
            fee = Tiers[i].fee * quantity;
            break;
        }
    }

    if (tierCrossed) 
    {
        fee = (Tiers[newTierIndex].fee * quantity) - rebate;
        return { true,fee };
    }
    else
        return { true,fee };
   

    return { false, fee };
}

template<typename T>
std::pair<bool, double> TieredMethod::calculateTieredFee(std::vector<Tier>& Tiers, T checkQty, int64_t quantity)
{
    double fee = 0.0;
    for (const auto& tier : Tiers)
    {
        if (checkQty <= tier.end)
        {

            fee = quantity * tier.fee;

            return { true, fee };
        }
    }

    return { false, fee };
}

