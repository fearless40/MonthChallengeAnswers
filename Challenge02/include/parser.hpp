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

template <class T>
concept is_parser = std::same_as<typename std::remove_cvref_t<T>::parser_tag, parser_tag>;

template <class T, class Begin_t, class End_t>
concept CanBeInitalizedByTwoiterators = requires(T a, Begin_t b, End_t e) {
    { T{b, e} } -> std::same_as<T>;
};

namespace attributes
{

template <class... T> using Attributes = std::tuple<T...>;

template <typename T>
concept has_attributes = T::attributes;

struct ws_never
{
};

struct ws_always
{
};

template <typename Attrib_t>
concept never_calls_ws = requires(Attrib_t attrib) {
    { attrib.attributes };
    { std::get<ws_never>(attrib.attributes) } -> std::convertible_to<ws_never>;
};

} // namespace attributes

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

namespace matchers
{

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

template <CharIterator begin_t, typename end_t> constexpr bool &match_white_space_not_endl(begin_t &begin, end_t &end)
{
    bool matched = false;
    while (begin != end && (*begin == '\0' || *begin == ' ' || *begin == '\t'))
    {
        ++begin;
        matched = true;
    }
    return matched;
}

template <CharIterator begin_t, typename end_t> constexpr bool &match_numbers(begin_t &begin, end_t &end)
{
    bool matched = false;
    while (begin != end && *begin >= '0' && *begin <= '9')
    {
        ++begin;
        matched = true;
    }

    return matched;
}

template <CharIterator begin_t, typename end_t> constexpr bool &match_lcase_letters(begin_t &begin, end_t &end)
{
    bool matched = false;
    while (begin != end && (to_lower(*begin) >= 'a' && to_lower(*begin) <= 'z'))
    {
        ++begin;
        matched = true;
    }

    return matched;
}

template <CharIterator begin_t, typename end_t> constexpr begin_t &match_char(begin_t begin, end_t end, char letter)
{
    if (*begin == letter)
    {
        ++begin;
        return true;
    }
    return false;
}

} // namespace matchers
} // namespace detail

struct empty
{
    using parser_tag = detail::parser_tag;
    using attributes = detail::attributes::Attributes<detail::attributes::ws_never>;
    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        return true;
    }
};

struct white_space_not_endl
{
    using parser_tag = detail::parser_tag;
    using attributes = detail::attributes::Attributes<detail::attributes::ws_always>;

    template <class begin_t, class end_t, class wskip_t = empty>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t skip = empty{})
    {
        return detail::matchers::match_white_space_not_endl(begin, end);
    }
};

template <typename p1_t, typename p2_t> struct follows
{
    std::remove_cv_t<p1_t> p1;
    std::remove_cv_t<p2_t> p2;

    using parser_tag = detail::parser_tag;

    constexpr follows(p1_t parse1, p2_t parse2) : p1(parse1), p2(parse2) {};

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        if constexpr (detail::attributes::never_calls_ws<p1_t>)
        {
            ws.parse(begin, end, ws);
        }

        if (p1.parse(begin, end, ws))
        {
            if constexpr (detail::attributes::never_calls_ws<p2_t>)
            {
                ws.parse(begin, end, ws);
            }
            return p2.parse(begin, end, ws);
        }
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

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
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

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
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

template <typename array_t, typename parser_t> struct action_array_like
{
    array_t &value;
    parser_t parser;

    using parser_tag = detail::parser_tag;

    action_array_like(array_t &val, parser_t &&parser_wrapper) : value(val), parser(parser_wrapper)
    {
    }

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        begin_t start = begin;
        if (parser.parse(begin, end, ws))
        {
            value = array_t{start, begin};
            return true;
        }
        return false;
    }
};

template <typename value_t> struct int_parser
{

    using parser_tag = detail::parser_tag;
    using attributes = detail::attributes::Attributes<detail::attributes::ws_never>;
    using type = int_parser<value_t>;

    template <class begin_t, class end_t, class wskip_t> constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws)
    {
        return detail::matchers::match_numbers(begin, end);
    }

    template <typename invoke_t> constexpr auto operator()(invoke_t &&invokable)
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

struct letter_parser_ignore_case
{

    using parser_tag = detail::parser_tag;
    using attributes = detail::attributes::Attributes<detail::attributes::ws_never>;

    template <class begin_t, class end_t, class wskip_t> constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws)
    {
        return detail::matchers::match_lcase_letters(begin, end);
    }

    template <typename invoke_t> constexpr auto operator()(invoke_t &&invokable)
    {
        // static_assert(std::is_integral_v<std::remove_cvref_t<invoke_t>>);
        static_assert(std::is_invocable_v<invoke_t, std::string_view>);
        if constexpr (detail::CanBeInitalizedByTwoiterators<invoke_t, std::forward_iterator, std::forward_iterator>)
        {
            return action_int_ref(std::forward<invoke_t>(invokable), letter_parser_ignore_case{});
        }
        if constexpr (std::is_invocable_v<invoke_t, std::string_view>)
        {
            return action_invoke<invoke_t, letter_parser_ignore_case>(std::forward<invoke_t>(invokable),
                                                                      letter_parser_ignore_case{});
        }
    }
};

template <detail::is_parser p1_t, detail::is_parser p2_t> auto operator>>(p1_t &&p1, p2_t &&p2)
{
    return follows<p1_t, p2_t>(p1, p2);
};

template <typename stream_t, typename parser_t, typename wskip_t>
void parse(stream_t &&stream, parser_t parser, wskip_t ws = white_space_not_endl)
{
    auto begin = std::begin(stream);
    auto end = std::end(stream);
    parser.parse(begin, end, ws);
}

namespace dsl
{
template <typename T> constexpr parser::int_parser<T> int_parser = parser::int_parser<T>{};

constexpr parser::letter_parser_ignore_case letters_ignore_case = parser::letter_parser_ignore_case{};
} // namespace dsl

} // namespace parser
