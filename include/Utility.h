#pragma once
#include "BusinessObjects.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <cmath>

namespace Utility
{
    constexpr double EPSILON = 1e-9;

    inline std::time_t Stringtotime(std::string timeinhours)
    {
        tm t;
        // we dont care about the date
        t.tm_mday = 1;
        t.tm_mon = 0;
        t.tm_year = 2022 - 1900;
        t.tm_wday = 0;
        t.tm_yday = 0;
        t.tm_isdst = 0;
        t.tm_hour = stoi(timeinhours.substr(0, 2));
        t.tm_min = stoi(timeinhours.substr(3, 2));
        t.tm_sec = stoi(timeinhours.substr(6, 2));

        time_t time = mktime(&t);
        return time;
    }

    inline void SetPrePostHours(const std::string& timeinhours, OrderExecutionData& data)
    {
        if (timeinhours.substr(0, 5) < "09:30") //Before 9:30 PM
        {
            data.afterHours = false;
            data.beforeHours = true;
        }


        else if (timeinhours.substr(0, 5) >= "16:00")//After & At 4:00 PM
        {
            data.afterHours = true;
            data.beforeHours = false;
        }

        else //During 9:30 AM and 4:00 PM
        {
            data.afterHours = false;
            data.beforeHours = false;
        }
    }

    template <typename T>
    std::string NumberToString(T Number)
    {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }

    template <typename T>
    T StringToNumber(const std::string& Text)
    {
        std::istringstream ss(Text);
        T result;
        return ss >> result ? result : 0;
    }

    inline double RoundOff(double value, int64_t prec, RoundingScheme rScheme)
    {
        double pow_10 = std::pow(10.0, (double)prec);

        switch (rScheme)
        {
        case EN_RoundingScheme_HalfUp:
            return std::floor((value * pow_10) + 0.5) / pow_10;
        case EN_RoundingScheme_HalfDown:
            return std::ceil((value * pow_10) - 0.5) / pow_10;
        case EN_RoundingScheme_AlwaysUp:
            return std::ceil(value * pow_10) / pow_10;
        case EN_RoundingScheme_AlwaysDown:
            return std::floor(value * pow_10) / pow_10;
        case EN_RoundingScheme_Truncate:
            return int(value * pow_10) / pow_10;
        default:
            return value;
        }

    }

    inline bool almostEqual(double a, double b, double eps = EPSILON) {
        return std::fabs(a - b) < eps;
    }

    inline bool almostNotEqual(double a, double b, double eps = EPSILON) {
        return !almostEqual(a, b, eps);
    }

    inline bool almostLess(double a, double b, double eps = EPSILON) {
        return (b - a) > eps;
    }

    inline bool almostLessOrEqual(double a, double b, double eps = EPSILON) {
        return (a < b) || almostEqual(a, b, eps);
    }

    inline bool almostGreater(double a, double b, double eps = EPSILON) {
        return (a - b) > eps;
    }

    inline bool almostGreaterOrEqual(double a, double b, double eps = EPSILON) {
        return (a > b) || almostEqual(a, b, eps);
    }

}
