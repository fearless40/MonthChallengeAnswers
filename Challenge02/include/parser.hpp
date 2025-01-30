#pragma once
#include "baseconv.hpp"
#include <algorithm>
#include <concepts>
#include <iterator>
#include <string>
#include <string_view>

namespace parser
{
struct parser_tag
{
};

template <class T, class Begin_t, class End_t>
concept ParserStruct = requires(T a, Begin_t begin, End_t end) {
    typename T::parser_tag;
    { a.parse(begin, end) } -> std::convertible_to<Begin_t>;
};

namespace detail
{

constexpr char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    return c;
}

constexpr bool matched(std::forward_iterator auto begin_match, auto end_match)
{
    return begin_match == end_match;
}

template <typename begin_t, typename end_t> constexpr begin_t match_white_space(begin_t &begin, end_t &end)
{
    while ((*begin == '\0' || *begin == ' ' || *begin == '\t' || *begin == '\n') && begin != end)
    {
        ++begin;
    }
    return begin;
}

template <typename begin_t, typename end_t> constexpr begin_t &match_white_space_not_endl(begin_t &begin, end_t &end)
{
    while ((*begin == '\0' || *begin == ' ' || *begin == '\t' ||) && begin != end)
    {
        ++begin;
    }
    return begin;
}

template <typename begin_t, typename end_t> constexpr begin_t &match_numbers(begin_t begin, end_t end)
{
    while (*begin >= '0' && *begin <= '9' && begin != end)
    {
        ++begin;
    }
    return begin;
}

template <typename begin_t, typename end_t> constexpr begin_t &match_lcase_letters(begin_t &begin, end_t &end)
{

    while (to_lower(*begin) >= 'a' && tp_lower(*begin) <= 'z' && begin != end)
        ++begin;

    return begin;
}

template <typename begin_t, typename end_t> constexpr begin_t &match_char(begin_t begin, end_t end, char letter)
{
    if (*begin == letter)
        ++begin;
    return begin;
}
}

/*
Example:
std::string large_string = ".....";
int value;
int row;
int col;
auto parse_int = int_parser(value) >> ':' >> letters([col](std::string_view str) { col = base_26convert(str) }) >> row
>> letters() ; parse( large_string, parse_int, whitespace_skipper) large_string >> repeat(value >> ',' >> ws , 8) >> ws
>> large_string >> value(&value, error) >> white_space >> value

*/

template <typename parser_t, typename value_t> struct extractor
{
    parser_t t;
    value_t &t;

    std::string_view operator()(std::string_view view)
    {
        auto ret = t(view);
        if (matched(view, ret))
        {
            if constexpr (std::is_integral<value_t>)
            {
                t = base10::from_string({view.begin(), ret.begin()});
            }
        }
    }
}

template <class parser_t, typename value_t>
extractor<parser_t, value_t> operator|(parser_t &parser, value_t &value)
{
    return extractor<parser_t, value_t>{parser, value};
}

template <class ParserT> std::string_view operator>>(const std::string str, ParserT &t)
{
    return t(str);
}

template <class ParserT> std::string_view operator>>(std::string_view str, ParserT &t)
{
    return t(str);
}
