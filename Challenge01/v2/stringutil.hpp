#pragma once

#include <algorithm>
#include <iterator>
#include <string>

namespace util
{

inline char toLower(char l)
{
    return std::tolower(static_cast<unsigned char>(l));
};

inline std::string copy_lower(const std::string_view str)
{
    std::string lcase;
    lcase.reserve(str.size());
    std::transform(str.begin(), str.end(), std::back_inserter(lcase), toLower);
    return lcase;
}

inline std::string &inplace_lower(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), toLower);
    return str;
}
}