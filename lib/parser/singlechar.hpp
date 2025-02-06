#pragma once
#include "actions.hpp"
#include "attributes.hpp"
#include "matchers.hpp"

namespace parser
{

template <char letter> struct single_char
{

    using parser_tag = attributes::parser_tag;
    using attributes = attributes::Attributes<attributes::ws_never>;
    using type = single_char<letter>;

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        return matchers::match_char(begin, end, letter);
    }

    template <typename invoke_t> constexpr auto operator()(invoke_t &&invokable) const noexcept
    {

        if constexpr (std::is_invocable_v<invoke_t, std::string_view>)
        {
            return action_invoke<invoke_t, letter_parser_ignore_case>(std::forward<invoke_t>(invokable), type{});
        }
        else
        //(detail::ConstructorTakesTwoIterators<invoke_t, std::forward_iterator, std::forward_iterator>)
        {
            return action_array_like(std::forward<invoke_t>(invokable), type{});
        }
    }
};
} // namespace parser