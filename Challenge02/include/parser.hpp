#pragma once
#include <algorithm>
#include <concepts>
#include <iterator>
#include <string>
#include <string_view>
#include "baseconv.hpp"

namespace parser
{

struct match_white_space
{

    template <typename begin_t, typename end_t>
    requires std::forward_iterator<begin_t> && std::equality_compare_with<begin_t, end_t>
    constexpr auto parse(begin_t begin, end_t end) -> begin_t
    {
        // char *ret = begin;
        while ((*begin == '\0' || *begin == ' ' || *begin == '\t' || *begin == '\n') && begin != end)
        {
            ++begin;
        }
        return begin;
    }

    std::string_view operator()(std::string_view view) const
    {
        auto next = parse(view.begin(), view.end());
        std::string_view(next, std::distance(next, view.end()));
    }
};


/*
Example:
std::string large_string = ".....";
int value;
large_string >> value  >> match_white_space{}  
large_string >> repeat(value >> ',' >> ws , 8) >> ws >>    
large_string >> value(&value, error) >> white_space >> value

*/

constexpr bool matched( std::string_view begin_match, std::string end_match ) {
    return begin_match.begin() != end_match.begin(); 
}

template<typename parser_t, typename value_t>
struct extractor {
    parser_t t;
    value_t & t; 
    
    std::string_view operator ()( std::string_view view ) {
        auto ret = t(view);
        if( matched( view ,ret) ) {
            if constexpr (std::is_integral<value_t>) {
                t = base10::from_string( {view.begin(), ret.begin() });
            }
        }
    }
}

struct match_numbers
{
    std::string_view matched_chars; 

    template <typename begin_t, typename end_t>
    requires std::forward_iterator<begin_t> && std::equality_compare_with<begin_t, end_t>
    constexpr auto parse(begin_t begin, end_t end) -> begin_t
    {
        while (*begin >= '0' && *begin <= '9' && begin != end)
        {
            ++begin;
        }
        return begin;
    }

    std::string_view operator()(std::string_view v)
    {
        auto next = parse(v.begin(), v.end());
        matched_chars = std::string_view{ v.begin(), next}; 
        return {next, v.end()};
    }
   
};



template <class parser_t, typename value_t> 
extractor<parser_t, value_t> operator | (parser_t & parser, value_t & value) {
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

constexpr char *match_char(char *next, char letter)
{
    char *ret = next;
    if (*ret == letter)
        return ++ret;
}
