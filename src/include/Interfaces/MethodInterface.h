#pragma once
#include "businessObjects.h"

namespace LSL 
{
	namespace FeeModule 
	{
		namespace Interfaces 
		{
			class MethodInterface 
			{
				private:
					virtual void parse_json(std::string& formula) = 0;

				public:
					virtual std::pair<bool, double> evaluate(const OrderExecutionData&) = 0;
			};
		}
	}
}