#include "JsonParser.h"
#include <string>
#include <stdexcept>
#include <cctype>
#include <algorithm>

inline std::string safeGetString(const rapidjson::Value* val) {
    if (val && val->IsString()) return val->GetString();
    return {};
}

inline double safeGetDouble(const rapidjson::Value* val) {
    if (val) {
        if (val->IsNumber()) return val->GetDouble();
        if (val->IsString()) {
            try { return std::stod(val->GetString()); }
            catch (...) {}
        }
    }
    return 0.0;
}

inline int64_t safeGetInt64(const rapidjson::Value* val) {
    if (val) {
        if (val->IsInt64()) return val->GetInt64();
        if (val->IsString()) {
            try { return std::stoll(val->GetString()); }
            catch (...) {}
        }
    }
    return 0;
}

inline int safeGetInt(const rapidjson::Value* val) {
    if (val) {
        if (val->IsInt()) return val->GetInt();
        if (val->IsString()) {
            try { return std::stoi(val->GetString()); }
            catch (...) {}
        }
    }
    return 0;
}

inline bool safeGetBool(const rapidjson::Value* val) {
    if (val && val->IsBool()) return val->GetBool();
    return false;
}
const rapidjson::Value* findMemberCaseInsensitive(const rapidjson::Value& obj, const std::string& name) {
    if (!obj.IsObject()) return nullptr;

    for (auto itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr) {
        std::string key = itr->name.GetString();
        if (key.size() == name.size() &&
            std::equal(key.begin(), key.end(), name.begin(),
                [](char a, char b) { return std::tolower(a) == std::tolower(b); })) {
            return &itr->value;
        }
    }
    return nullptr;
}

void makeOrder(rapidjson::Document& doc, std::priority_queue<OrderExecutionData>& orderData)
{
    if (doc.IsObject() && doc.HasMember("Executions") && doc["Executions"].IsArray() && !doc["Executions"].Empty()) {
        if (doc.IsObject() && doc.HasMember("Executions") && doc["Executions"].IsArray()) {
            for (const auto& exec : doc["Executions"].GetArray()) {
                OrderExecutionData data;

                data.id = safeGetString(findMemberCaseInsensitive(exec, "Id"));
                data.execBroker = safeGetString(findMemberCaseInsensitive(exec, "ExecBroker"));
                data.contra = safeGetString(findMemberCaseInsensitive(exec, "Contra"));
                data.route = safeGetString(findMemberCaseInsensitive(exec, "Route"));
                data.internalRoute = safeGetString(findMemberCaseInsensitive(exec, "InternalRoute"));
                data.liq = safeGetString(findMemberCaseInsensitive(exec, "Liq"));
                data.internalLiq = safeGetString(findMemberCaseInsensitive(exec, "InternalLiq"));
                data.dst = safeGetString(findMemberCaseInsensitive(exec, "DST"));
                data.multiplier = safeGetDouble(findMemberCaseInsensitive(exec, "Mult")); // need to check exact name from executionData
                data.tape = safeGetString(findMemberCaseInsensitive(exec, "Tape"));
                data.price = safeGetDouble(findMemberCaseInsensitive(exec, "Price"));
                data.quantity = safeGetDouble(findMemberCaseInsensitive(exec, "Quantity"));
                data.monthlyVolume = safeGetDouble(findMemberCaseInsensitive(exec, "MonthlyVolume"));
                data.mpidMonthlyVolume = safeGetDouble(findMemberCaseInsensitive(exec, "MPIDMonthlyVolume"));
                data.firmMonthlyVolume = safeGetDouble(findMemberCaseInsensitive(exec, "FirmMonthlyVolume"));
                data.beforeHours = safeGetBool(findMemberCaseInsensitive(exec, "BeforeHours"));
                data.afterHours = safeGetBool(findMemberCaseInsensitive(exec, "AfterHours"));
                data.penny = safeGetBool(findMemberCaseInsensitive(exec, "Penny"));
                data.time = safeGetString(findMemberCaseInsensitive(exec, "Time"));
                data.ExchangeType = safeGetInt(findMemberCaseInsensitive(exec, "ExchangeType"));
                data.type = safeGetInt(findMemberCaseInsensitive(exec, "Type"));
                data.side = safeGetInt(findMemberCaseInsensitive(exec, "Side"));
                //data.lot = findMemberCaseInsensitive(exec, "Lot")); // need to discuss data type with talha bhai
                data.lastMarket = safeGetString(findMemberCaseInsensitive(exec, "LastMarket")); // need to discuss data type with talha bhai
                data.date = safeGetString(findMemberCaseInsensitive(exec, "Date"));
                data.accountValue = safeGetString(findMemberCaseInsensitive(exec, "AccountValue"));
                data.orderId = safeGetString(findMemberCaseInsensitive(exec, "OrderId"));
                data.executionId = safeGetString(findMemberCaseInsensitive(exec, "ExecutionId"));

                //data.accountId = safeGetString(findMemberCaseInsensitive(exec, "AccountId")); // Need to check required or not
                //data.symbol = safeGetString(findMemberCaseInsensitive(exec, "Symbol"));
                //data.currency = safeGetString(findMemberCaseInsensitive(exec, "Currency"));// Need to check
                //data.avgPx = safeGetDouble(findMemberCaseInsensitive(exec, "AvgPx"));
                //data.lastPx = safeGetDouble(findMemberCaseInsensitive(exec, "LastPx"));
                //data.execQty = safeGetDouble(findMemberCaseInsensitive(exec, "ExecQty"));
                //data.lastShares = safeGetDouble(findMemberCaseInsensitive(exec, "LastShares"));
                //data.capacity = safeGetInt(findMemberCaseInsensitive(exec, "Capacity")); // Need to check required or not

                //
                //data.firmId = safeGetString(findMemberCaseInsensitive(exec, "FirmId")); // Need to check required or not
                //data.fileName = safeGetString(findMemberCaseInsensitive(exec, "FileName")); // Need to check required or not
                //data.mpidRecv = safeGetString(findMemberCaseInsensitive(exec, "MPIDRecv")); 


                orderData.push(data);
            }
        }
    }
}
void makePlans(rapidjson::Document& doc, std::vector<PlanData>& plansQueue) {

    if (doc.IsObject() && doc.HasMember("Plans") && doc["Plans"].IsArray()) {
        for (const auto& plan : doc["Plans"].GetArray()) {
            PlanData p;
            p.id = safeGetString(findMemberCaseInsensitive(plan, "Id"));
            p.name = safeGetString(findMemberCaseInsensitive(plan, "Name"));
            p.rounding = safeGetInt64(findMemberCaseInsensitive(plan, "Rounding"));
            p.roundingPoints = safeGetInt64(findMemberCaseInsensitive(plan, "RoundingPoints"));
            p.formulaType = safeGetInt64(findMemberCaseInsensitive(plan, "FormulaType"));
            p.planType = safeGetInt64(findMemberCaseInsensitive(plan, "PlanType"));

            //p.firmId = safeGetString(findMemberCaseInsensitive(plan, "FirmId")); // Need to check required or not
            //p.created = safeGetString(findMemberCaseInsensitive(plan, "Created"));
            //p.createdBy = safeGetString(findMemberCaseInsensitive(plan, "CreatedBy"));
            //p.modified = safeGetString(findMemberCaseInsensitive(plan, "Modified"));
            //p.modifiedBy = safeGetString(findMemberCaseInsensitive(plan, "ModifiedBy"));
            //p.isDeleted = safeGetBool(findMemberCaseInsensitive(plan, "IsDeleted"));
            //p.isSystemGenerated = safeGetBool(findMemberCaseInsensitive(plan, "IsSystemGenerated"));
            //p.isActive = safeGetBool(findMemberCaseInsensitive(plan, "IsActive"));
            //p.feeCategoryId = safeGetString(findMemberCaseInsensitive(plan, "FeeCategoryId"));
            //p.isCompiled = safeGetBool(findMemberCaseInsensitive(plan, "IsCompiled"));
            //p.feeCategoryType = safeGetInt64(findMemberCaseInsensitive(plan, "FeeCategoryType"));
            //p.incrementalPlanId = safeGetInt64(findMemberCaseInsensitive(plan, "IncrementalPlanId"));
            //p.parentPlanId = safeGetInt64(findMemberCaseInsensitive(plan, "ParentPlanId"));

            // Handle Formula as JSON object → string
            if (const auto* val = findMemberCaseInsensitive(plan, "Formula")) {
                if (val->IsObject() || val->IsArray()) {
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    val->Accept(writer);
                    p.formula_final = buffer.GetString();
                }
                else if (val->IsString()) {
                    p.formula_final = val->GetString();
                }
            }

            plansQueue.push_back(std::move(p));

        }
    }
}
//void makeOrder(rapidjson::Document& doc, std::priority_queue<OrderExecutionData>& orderData) {
//    if (doc.IsObject() && doc.HasMember("Executions") && doc["Executions"].IsArray() && !doc["Executions"].Empty()) {
//        if (doc.IsObject() && doc.HasMember("Executions") && doc["Executions"].IsArray()) {
//            for (const auto& exec : doc["Executions"].GetArray()) {
//                OrderExecutionData data;
//
//                // Map fields 
//                //if (exec.HasMember("OrderId") && exec["OrderId"].IsString())
//                if (const auto* val = findMemberCaseInsensitive(exec, "OrderId"))
//                    if (val->IsString()) data.orderId = val->GetString();
//
//                //if (exec.HasMember("Id") && exec["Id"].IsInt64())
//                if (const auto* val = findMemberCaseInsensitive(exec, "Id"))
//                    if (val->IsString()) data.id = val->GetString();
//
//                //if (exec.HasMember("ExecutionId") && exec["ExecutionId"].IsString())
//                if (const auto* val = findMemberCaseInsensitive(exec, "ExecutionId"))
//                    if (val->IsString()) data.executionId = val->GetString();
//
//                //if (exec.HasMember("Price") && exec["Price"].IsNumber())
//                if (const auto* val = findMemberCaseInsensitive(exec, "Price"))
//                    if (val->IsNumber()) data.price = val->GetDouble();
//
//                //if (exec.HasMember("Quantity") && exec["Quantity"].IsInt64())
//                if (const auto* val = findMemberCaseInsensitive(exec, "Quantity"))
//                    if (val->IsInt64()) data.quantity = val->GetInt64();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "ExecBroker"))
//                    if (val->IsString()) data.execBroker = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Contra"))
//                    if (val->IsString()) data.contra = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Symbol"))
//                    if (val->IsString()) data.symbol = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Currency"))
//                    if (val->IsString()) data.currency = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "AvgPx"))
//                    if (val->IsInt64()) data.avgPx = val->GetInt64();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "LastPx"))
//                    if (val->IsInt64()) data.lastPx = val->GetInt64();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "ExecQty"))
//                    if (val->IsInt64()) data.execQty = val->GetInt64();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "LastShares"))
//                    if (val->IsInt64()) data.lastShares = val->GetInt64();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Penny"))
//                    if (val->IsBool()) data.penny = val->GetBool();
//                    //data.penny = exec["Penny"].GetBool();          
//                if (const auto* val = findMemberCaseInsensitive(exec, "AfterHours"))
//                    if (val->IsBool()) data.afterHours = val->GetBool();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "BeforeHours"))
//                    if (val->IsBool()) data.beforeHours = val->GetBool();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Lot"))
//                    if (val->IsString()) data.lot = std::stoi(val->GetString());
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Side"))
//                    if (val->IsString()) data.side = std::stoi(val->GetString());
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Capacity"))
//                    if (val->IsString()) data.capacity = std::stoi(val->GetString());
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Route"))
//                    if (val->IsString()) data.route = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Liq"))
//                    if (val->IsString()) data.liq = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "InternalRoute"))
//                    if (val->IsString()) data.internalRoute = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "InternalLiq"))
//                    if (val->IsString()) data.internalLiq = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Tape"))
//                    if (val->IsString()) data.tape = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "DST"))
//                    if (val->IsString()) data.dst = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Date"))
//                    if (val->IsString()) data.date = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Time"))
//                    if (val->IsString()) data.time = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "AccountId"))
//                    if (val->IsString()) data.accountId = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "AccountValue"))
//                    if (val->IsString()) data.accountValue = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "FirmId"))
//                    if (val->IsString()) data.firmId = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "FileName"))
//                    if (val->IsString()) data.fileName = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "MPIDRecv"))
//                    if (val->IsString()) data.mpidRecv = val->GetString();
//
//                if (const auto* val = findMemberCaseInsensitive(exec, "Type"))
//                    if (val->IsString()) data.type = std::stoi(val->GetString());
//                orderData.push(data);
//            }
//        }
//    }
//}

//void makePlans(rapidjson::Document& doc, std::vector<PlanData>& plansQueue) {
//
//    if (doc.IsObject() && doc.HasMember("Plans") && doc["Plans"].IsArray()) {
//        for (const auto& plan : doc["Plans"].GetArray()) {
//            PlanData p;
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "Id"))
//                if (val->IsString()) p.id = val->GetString();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "Name"))
//                if (val->IsString()) p.name = val->GetString();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "Rounding"))
//                if (val->IsInt64()) p.rounding = val->GetInt64();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "RoundingPoints"))
//                if (val->IsInt64()) p.roundingPoints = val->GetInt64();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "PlanType"))
//                if (val->IsInt64()) p.planType = val->GetInt64();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "FirmId"))
//                if (val->IsString()) p.firmId = val->GetString();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "Created"))
//                if (val->IsString()) p.created = val->GetString();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "CreatedBy"))
//                if (val->IsString()) p.createdBy = val->GetString();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "Modified"))
//                if (val->IsString()) p.modified = val->GetString();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "ModifiedBy"))
//                if (val->IsString()) p.modifiedBy = val->GetString();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "IsDeleted"))
//                if (val->IsBool()) p.isDeleted = val->GetBool();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "IsSystemGenerated"))
//                if (val->IsBool()) p.isSystemGenerated = val->GetBool();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "IsActive"))
//                if (val->IsBool()) p.isActive = val->GetBool();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "FeeCategoryId"))
//                if (val->IsString()) p.feeCategoryId = val->GetString();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "FormulaType"))
//                if (val->IsInt64()) p.formulaType = val->GetInt64();
//
//            if (plan.HasMember("Formula")) {
//                rapidjson::StringBuffer buffer;
//                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//                plan["Formula"].Accept(writer);
//                p.formula_final = buffer.GetString();
//            }
//            if (const auto* val = findMemberCaseInsensitive(plan, "IsCompiled"))
//                if (val->IsBool()) p.isCompiled = val->GetBool();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "FeeCategoryType"))
//                if (val->IsBool()) p.feeCategoryType = val->GetBool();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "IncrementalPlanId"))
//                if (val->IsInt64()) p.incrementalPlanId = val->GetInt64();
//
//            if (const auto* val = findMemberCaseInsensitive(plan, "ParentPlanId"))
//                if (val->IsInt64()) p.parentPlanId = val->GetInt64();
//
//            plansQueue.push_back(p);
//        }
//    }
//}