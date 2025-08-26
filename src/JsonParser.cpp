#include "JsonParser.h"

void makeOrder(rapidjson::Document& doc, std::queue<OrderExecutionData>& orderData) {
    if (doc.IsObject() && doc.HasMember("Executions") && doc["Executions"].IsArray() && !doc["Executions"].Empty()) {
        if (doc.IsObject() && doc.HasMember("Executions") && doc["Executions"].IsArray()) {
            for (const auto& exec : doc["Executions"].GetArray()) {
                OrderExecutionData data;

                // Map fields 
                if (exec.HasMember("OrderId") && exec["OrderId"].IsString())
                    data.orderId = exec["OrderId"].GetString();

                if (exec.HasMember("ExecutionId") && exec["ExecutionId"].IsString())
                    data.executionId = exec["ExecutionId"].GetString();

                if (exec.HasMember("Price") && exec["Price"].IsNumber())
                    data.price = exec["Price"].GetDouble();

                if (exec.HasMember("Quantity") && exec["Quantity"].IsInt64())
                    data.quantity = exec["Quantity"].GetInt64();

                if (exec.HasMember("ExecBroker") && exec["ExecBroker"].IsString())
                    data.execBroker = exec["ExecBroker"].GetString();

                if (exec.HasMember("Contra") && exec["Contra"].IsString())
                    data.contra = exec["Contra"].GetString();

                if (exec.HasMember("Symbol") && exec["Symbol"].IsString())
                    data.symbol = exec["Symbol"].GetString();

                if (exec.HasMember("Currency") && exec["Currency"].IsString())
                    data.currency = exec["Currency"].GetString();

                if (exec.HasMember("AvgPx") && exec["AvgPx"].IsNumber())
                    data.avgPx = exec["AvgPx"].GetDouble();

                if (exec.HasMember("LastPx") && exec["LastPx"].IsNumber())
                    data.lastPx = exec["LastPx"].GetDouble();

                if (exec.HasMember("ExecQty") && exec["ExecQty"].IsInt64())
                    data.execQty = exec["ExecQty"].GetInt64();

                if (exec.HasMember("LastShares") && exec["LastShares"].IsInt64())
                    data.lastShares = exec["LastShares"].GetInt64();

                if (exec.HasMember("Penny") && exec["Penny"].IsBool())
                    data.penny = exec["Penny"].GetBool();

                if (exec.HasMember("AfterHours") && exec["AfterHours"].IsBool())
                    data.afterHours = exec["AfterHours"].GetBool();

                if (exec.HasMember("BeforeHours") && exec["BeforeHours"].IsBool())
                    data.beforeHours = exec["BeforeHours"].GetBool();

                if (exec.HasMember("Lot") && exec["Lot"].IsString())
                    data.lot = std::stoi(exec["Lot"].GetString());

                if (exec.HasMember("Side") && exec["Side"].IsString())
                    data.side = std::stoi(exec["Side"].GetString());

                if (exec.HasMember("Capacity") && exec["Capacity"].IsString())
                    data.capacity = std::stoi(exec["Capacity"].GetString());

                if (exec.HasMember("Route") && exec["Route"].IsString())
                    data.route = exec["Route"].GetString();

                if (exec.HasMember("Liq") && exec["Liq"].IsString())
                    data.liq = exec["Liq"].GetString();

                if (exec.HasMember("InternalRoute") && exec["InternalRoute"].IsString())
                    data.internalRoute = exec["InternalRoute"].GetString();

                if (exec.HasMember("InternalLiq") && exec["InternalLiq"].IsString())
                    data.internalLiq = exec["InternalLiq"].GetString();

                if (exec.HasMember("Tape") && exec["Tape"].IsString())
                    data.tape = exec["Tape"].GetString();

                if (exec.HasMember("DST") && exec["DST"].IsString())
                    data.dst = exec["DST"].GetString();

                if (exec.HasMember("Date") && exec["Date"].IsString())
                    data.date = exec["Date"].GetString();

                if (exec.HasMember("Time") && exec["Time"].IsString())
                    data.time = exec["Time"].GetString();

                if (exec.HasMember("AccountId") && exec["AccountId"].IsString())
                    data.accountId = exec["AccountId"].GetString();

                if (exec.HasMember("AccountValue") && exec["AccountValue"].IsString())
                    data.accountValue = exec["AccountValue"].GetString();

                if (exec.HasMember("FirmId") && exec["FirmId"].IsString())
                    data.firmId = exec["FirmId"].GetString();

                if (exec.HasMember("FileName") && exec["FileName"].IsString())
                    data.fileName = exec["FileName"].GetString();

                if (exec.HasMember("MPIDRecv") && exec["MPIDRecv"].IsString())
                    data.mpidRecv = exec["MPIDRecv"].GetString();

                if (exec.HasMember("Type") && exec["Type"].IsString())
                    data.type = std::stoi(exec["Type"].GetString());

                orderData.push(data);
            }
        }
    }
}

void makePlans(rapidjson::Document& doc, std::vector<PlanData>& plansQueue) {

    if (doc.IsObject() && doc.HasMember("Plans") && doc["Plans"].IsArray()) {
        for (const auto& plan : doc["Plans"].GetArray()) {
            PlanData p;

            if (plan.HasMember("Id") && plan["Id"].IsString())
                p.id = plan["Id"].GetString();

            if (plan.HasMember("Name") && plan["Name"].IsString())
                p.name = plan["Name"].GetString();

            if (plan.HasMember("Rounding") && plan["Rounding"].IsInt())
                p.rounding = plan["Rounding"].GetInt();

            if (plan.HasMember("RoundingPoints") && plan["RoundingPoints"].IsInt())
                p.roundingPoints = plan["RoundingPoints"].GetInt();

            if (plan.HasMember("PlanType") && plan["PlanType"].IsInt())
                p.planType = plan["PlanType"].GetInt();

            if (plan.HasMember("FirmId") && plan["FirmId"].IsString())
                p.firmId = plan["FirmId"].GetString();

            if (plan.HasMember("Created") && plan["Created"].IsString())
                p.created = plan["Created"].GetString();

            if (plan.HasMember("CreatedBy") && plan["CreatedBy"].IsString())
                p.createdBy = plan["CreatedBy"].GetString();

            if (plan.HasMember("Modified") && plan["Modified"].IsString())
                p.modified = plan["Modified"].GetString();

            if (plan.HasMember("ModifiedBy") && plan["ModifiedBy"].IsString())
                p.modifiedBy = plan["ModifiedBy"].GetString();

            if (plan.HasMember("IsDeleted") && plan["IsDeleted"].IsBool())
                p.isDeleted = plan["IsDeleted"].GetBool();

            if (plan.HasMember("IsSystemGenerated") && plan["IsSystemGenerated"].IsBool())
                p.isSystemGenerated = plan["IsSystemGenerated"].GetBool();

            if (plan.HasMember("IsActive") && plan["IsActive"].IsBool())
                p.isActive = plan["IsActive"].GetBool();

            if (plan.HasMember("FeeCategoryId") && plan["FeeCategoryId"].IsString())
                p.feeCategoryId = plan["FeeCategoryId"].GetString();

            if (plan.HasMember("FormulaType") && plan["FormulaType"].IsInt())
                p.formulaType = plan["FormulaType"].GetInt();

            if (plan.HasMember("Formula")) {
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                plan["Formula"].Accept(writer);
                p.formula_final = buffer.GetString();
                const auto& formula = plan["Formula"];
                if (formula.IsObject()) {  // Tiered formula
                    if (formula.HasMember("TieredType") && formula["TieredType"].IsInt())
                        p.formula.tieredType = formula["TieredType"].GetInt();

                    if (formula.HasMember("isRegressive") && formula["isRegressive"].IsBool())
                        p.formula.isRegressive = formula["isRegressive"].GetBool();

                    if (formula.HasMember("TieredTable") && formula["TieredTable"].IsArray()) {
                        for (const auto& entry : formula["TieredTable"].GetArray()) {
                            TieredTableEntry t;
                            if (entry.HasMember("StartRange") && entry["StartRange"].IsNumber())
                                t.startRange = entry["StartRange"].GetDouble();

                            if (entry.HasMember("EndRange") && entry["EndRange"].IsNumber())
                                t.endRange = entry["EndRange"].GetDouble();

                            if (entry.HasMember("Fee") && entry["Fee"].IsString())
                                t.fee = entry["Fee"].GetString();

                            if (entry.HasMember("SequenceId") && entry["SequenceId"].IsInt())
                                t.sequenceId = entry["SequenceId"].GetInt();

                            p.formula.tieredTable.push_back(t);
                        }
                    }
                }
                else if (formula.IsArray()) {  // FormulaType = 1 case
                    for (const auto& f : formula.GetArray()) {
                        ConditionFormula cf;

                        if (f.HasMember("SequenceId") && f["SequenceId"].IsInt())
                            cf.sequenceId = f["SequenceId"].GetInt();

                        if (f.HasMember("ExecBroker") && f["ExecBroker"].IsString())
                            cf.execBroker = f["ExecBroker"].GetString();

                        if (f.HasMember("Contra") && f["Contra"].IsString())
                            cf.contra = f["Contra"].GetString();

                        if (f.HasMember("Route") && f["Route"].IsString())
                            cf.route = f["Route"].GetString();

                        if (f.HasMember("InternalRoute") && f["InternalRoute"].IsString())
                            cf.internalRoute = f["InternalRoute"].GetString();

                        if (f.HasMember("LIQ") && f["LIQ"].IsString())
                            cf.liq = f["LIQ"].GetString();

                        if (f.HasMember("DST") && f["DST"].IsString())
                            cf.dst = f["DST"].GetString();

                        if (f.HasMember("Tape") && f["Tape"].IsString())
                            cf.tape = f["Tape"].GetString();

                        if (f.HasMember("Price") && f["Price"].IsNumber())
                            cf.price = f["Price"].GetDouble();

                        if (f.HasMember("Quantity") && f["Quantity"].IsInt64())
                            cf.quantity = f["Quantity"].GetInt64();

                        if (f.HasMember("BeforeHours") && f["BeforeHours"].IsBool())
                            cf.beforeHours = f["BeforeHours"].GetBool();

                        if (f.HasMember("AfterHours") && f["AfterHours"].IsBool())
                            cf.afterHours = f["AfterHours"].GetBool();

                        if (f.HasMember("Penny") && f["Penny"].IsBool())
                            cf.penny = f["Penny"].GetBool();

                        if (f.HasMember("Time") && f["Time"].IsString())
                            cf.time = f["Time"].GetString();

                        if (f.HasMember("Type") && f["Type"].IsInt())
                            cf.type = f["Type"].GetInt();

                        if (f.HasMember("Side") && f["Side"].IsInt())
                            cf.side = f["Side"].GetInt();

                        if (f.HasMember("InternalLiq") && f["InternalLiq"].IsString())
                            cf.internalLiq = f["InternalLiq"].GetString();

                        if (f.HasMember("Lot") && f["Lot"].IsString())
                            cf.lot = f["Lot"].GetString();

                        if (f.HasMember("MonthlyVolume") && f["MonthlyVolume"].IsInt64())
                            cf.monthlyVolume = f["MonthlyVolume"].GetInt64();

                        if (f.HasMember("Fee") && f["Fee"].IsString())
                            cf.fee = f["Fee"].GetString();

                        p.formula.conditions.push_back(cf);
                    }
                }
            }

            if (plan.HasMember("IsCompiled") && plan["IsCompiled"].IsBool())
                p.isCompiled = plan["IsCompiled"].GetBool();

            if (plan.HasMember("FeeCategoryType") && plan["FeeCategoryType"].IsInt())
                p.feeCategoryType = plan["FeeCategoryType"].GetInt();

            if (plan.HasMember("IncrementalPlanId") && plan["IncrementalPlanId"].IsInt())
                p.incrementalPlanId = plan["IncrementalPlanId"].GetInt();

            if (plan.HasMember("ParentPlanId") && plan["ParentPlanId"].IsInt())
                p.parentPlanId = plan["ParentPlanId"].GetInt();

            plansQueue.push_back(p);
        }
    }
}