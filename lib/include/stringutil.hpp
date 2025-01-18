#pragma once
#include <algorithm>
#include <iterator>
#include <string>

namespace util
{

inline char to_lower(char l)
{
    return std::tolower(static_cast<unsigned char>(l));
};

inline std::string to_lower_copy(const std::string_view str)
{
    std::string lcase;
    lcase.reserve(str.size());
    std::transform(str.begin(), str.end(), std::back_inserter(lcase), to_lower);
    return lcase;
}

inline std::string &to_lower_inplace(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), to_lower);
    return str;
}

namespace parser
{

constexpr char *match_white_space(char *next)
{
    char *ret = next;
    while (*ret == '\0' || *ret == ' ' || *ret == '\t')
    {
        ++ret;
    }
    return ret;
}

constexpr char *match_numbers(char *next)
{
    char *ret = next;
    while (*ret >= '0' && *ret <= '9')
    {
        ++ret;
    }
    return ret;
}

char *match_lcase_letters(char *next)
{
    char *ret = next;
    char lower = util::to_lower(*ret);

    while (lower >= 'a' && lower <= 'z')
    {
        ++ret;
        lower = util::to_lower(*ret);
    }
    return ret;
}
} // namespace parser

} // namespace util