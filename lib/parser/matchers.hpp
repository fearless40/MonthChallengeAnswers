#pragma once

#include "charutil.hpp"
#include "concepts.hpp"
#include <concepts>
#include <iterator>

namespace parser
{
namespace matchers
{
using namespace parser::concepts;

constexpr bool matched(std::forward_iterator auto begin_match, auto end_match)
{
    return begin_match == end_match;
}

template <CharIterator begin_t, typename end_t> constexpr bool match_white_space(begin_t &begin, end_t &end)
{
    bool matched = false;
    while (begin != end && (*begin == '\0' || *begin == ' ' || *begin == '\t' || *begin == '\n'))
    {
        ++begin;
        matched = true;
    }
    return matched;
}

template <CharIterator begin_t, typename end_t> constexpr bool match_white_space_not_endl(begin_t &begin, end_t &end)
{
    bool matched = false;
    while (begin != end && (*begin == '\0' || *begin == ' ' || *begin == '\t'))
    {
        ++begin;
        matched = true;
    }
    return matched;
}

template <CharIterator begin_t, typename end_t> constexpr bool match_numbers(begin_t &begin, end_t &end)
{
    bool matched = false;
    while (begin != end && *begin >= '0' && *begin <= '9')
    {
        ++begin;
        matched = true;
    }

    return matched;
}

template <CharIterator begin_t, typename end_t> constexpr bool match_lcase_letters(begin_t &begin, end_t &end)
{
    bool matched = false;
    while (begin != end && (util::to_lower(*begin) >= 'a' && util::to_lower(*begin) <= 'z'))
    {
        ++begin;
        matched = true;
    }

    return matched;
}

template <CharIterator begin_t, typename end_t> constexpr bool match_char(begin_t &begin, end_t end, char letter)
{
    if (begin != end && *begin == letter)
    {
        ++begin;
        return true;
    }
    return false;
}

} // namespace matchers
} // namespace parser
