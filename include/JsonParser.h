#pragma once
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <queue>
#include "BusinessObjects.h"

void makeOrder(rapidjson::Document& doc, std::queue<OrderExecutionData>& orderData);
void makePlans(rapidjson::Document& doc, std::vector<PlanData>& plansQueue);