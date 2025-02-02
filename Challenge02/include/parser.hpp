#pragma once
#include "baseconv.hpp"
#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>

namespace parser
{

namespace detail
{

struct parser_tag
{
};

template <class T, class Begin_t, class End_t, class wskip_t>
concept ParserStruct = requires(T a, Begin_t begin, End_t end, wskip_t wskip) {
    typename T::parser_tag;
    { a.parse(begin, end, wskip) } -> std::convertible_to<bool>;
};

template <class T>
concept CharIterator = requires(T a) {
    { *a } -> std::convertible_to<char>;
};

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

template <CharIterator begin_t, typename end_t> constexpr begin_t match_white_space(begin_t &begin, end_t &end)
{
    while (begin != end && (*begin == '\0' || *begin == ' ' || *begin == '\t' || *begin == '\n'))
    {
        ++begin;
    }
    return begin;
}

template <CharIterator begin_t, typename end_t>
constexpr begin_t &match_white_space_not_endl(begin_t &begin, end_t &end)
{
    while (begin != end && (*begin == '\0' || *begin == ' ' || *begin == '\t'))
    {
        ++begin;
    }
    return begin;
}

template <CharIterator begin_t, typename end_t> constexpr begin_t &match_numbers(begin_t &begin, end_t &end)
{
    std::cout << "\nNumbers: ";
    while (begin != end && *begin >= '0' && *begin <= '9')
    {
        std::cout << *begin;
        ++begin;
    }

    return begin;
}

template <CharIterator begin_t, typename end_t> constexpr begin_t &match_lcase_letters(begin_t &begin, end_t &end)
{

    while (begin != end && (to_lower(*begin) >= 'a' && to_lower(*begin) <= 'z'))
        ++begin;

    return begin;
}

template <CharIterator begin_t, typename end_t> constexpr begin_t &match_char(begin_t begin, end_t end, char letter)
{
    if (*begin == letter)
        ++begin;
    return begin;
}
} // namespace detail

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

struct empty
{
    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        return true;
    }
};

struct white_space_not_endl
{
    using parser_tag = detail::parser_tag;

    template <class begin_t, class end_t, class wskip_t = empty>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t skip = empty{})
    {
        detail::match_white_space_not_endl(begin, end);
        return true;
    }
};

template <typename p1_t, typename p2_t> struct follows
{
    p1_t p1;
    p2_t p2;

    using parser_tag = detail::parser_tag;
    follows(p1_t parse1, p2_t parse2) : p1(parse1), p2(parse2) {};

    template <class begin_t, class end_t, class wskip_t> constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws)
    {
        if (p1.parse(begin, end, ws))
            return p2.parse(begin, end, ws);
        return false;
    }
};

template <typename invokable_t, typename parser_t> struct action_invoke
{
    using parser_tag = detail::parser_tag;

    invokable_t invokable;
    parser_t parser;

    action_invoke(invokable_t &&invoke, parser_t &&parser_wrapper) : invokable(invoke), parser(parser_wrapper)
    {
    }

    template <class begin_t, class end_t, class wskip_t> constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws)
    {

        begin_t start = begin;
        if (parser.parse(begin, end, ws))
        {
            std::invoke(invokable, std::string_view{start, begin});
            return true;
        }
        return false;
    }
};

template <typename int_t, typename parser_t> struct action_int_ref
{
    int_t &value;
    parser_t parser;

    using parser_tag = detail::parser_tag;

    action_int_ref(int_t &val, parser_t &&parser_wrapper) : value(val), parser(parser_wrapper)
    {
    }

    template <class begin_t, class end_t, class wskip_t> constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws)
    {

        begin_t start = begin;
        if (parser.parse(begin, end, ws))
        {
            value = base10::from_string({start, begin});
            return true;
        }
        return false;
    }
};

template <typename value_t> struct int_parser
{

    using parser_tag = detail::parser_tag;
    using type = int_parser<value_t>;

    template <class begin_t, class end_t, class wskip_t> constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws)
    {

        ws.parse(begin, end);
        auto start = begin;
        detail::match_numbers(begin, end);

        if (start != begin)
            return true;

        return false;
    }

    template <typename invoke_t> constexpr auto operator[](invoke_t &&invokable)
    {
        // static_assert(std::is_integral_v<std::remove_cvref_t<invoke_t>>);
        static_assert(std::is_invocable_v<invoke_t, std::string_view>);
        if constexpr (std::is_integral_v<std::remove_cvref_t<invoke_t>>)
        {
            return action_int_ref(std::forward<invoke_t>(invokable), type{});
        }
        if constexpr (std::is_invocable_v<invoke_t, std::string_view>)
        {
            return action_invoke<invoke_t, type>(std::forward<invoke_t>(invokable), type{});
        }
    }
};

struct letter_parser
{

    std::string_view &matched;
    using parser_tag = detail::parser_tag;

    letter_parser(std::string_view &view) : matched(view) {};

    template <class begin_t, class end_t, class wskip_t> constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws)
    {

        ws.parse(begin, end);
        auto start = begin;
        detail::match_lcase_letters(begin, end);

        if (start != begin)
        {
            matched = std::string_view{start, begin};
            return true;
        }

        return false;
    }
};

/*template <typename parser_t, typename value_t> struct extractor
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
}*/

template <typename p1_t, typename p2_t> auto operator>>(p1_t &&p1, p2_t &&p2)
{
    return follows(p1, p2);
};

template <typename stream_t, typename parser_t, typename wskip_t>
void parse(stream_t &&stream, parser_t parser, wskip_t ws = white_space_not_endl)
{
    auto begin = std::begin(stream);
    auto end = std::end(stream);
    parser.parse(begin, end, ws);
}
} // namespace parser
