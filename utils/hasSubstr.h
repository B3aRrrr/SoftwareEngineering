#pragma once
#include <string>
#include "Poco/Base64Decoder.h"
#include <istream>
#include <ostream>


static bool hasSubstr(const std::string &str, const std::string &substr) // определение запроса для обработки
{
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}