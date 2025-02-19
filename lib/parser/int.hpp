#pragma once
#include "actions.hpp"
#include "attributes.hpp"
#include "matchers.hpp"

namespace parser
{

template <typename value_t> struct int_parser
{

    using parser_tag = attributes::parser_tag;
    using attributes = attributes::Attributes<attributes::ws_never>;
    using type = int_parser<value_t>;

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        return matchers::match_numbers(begin, end);
    }

    template <typename invoke_t> constexpr auto operator()(invoke_t &&invokable) const noexcept
    {
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
} // namespace parser