#pragma once

namespace parser
{

struct letter_parser_ignore_case
{

    using parser_tag = detail::parser_tag;
    using attributes = detail::attributes::Attributes<detail::attributes::ws_never>;

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        return detail::matchers::match_lcase_letters(begin, end);
    }

    template <typename invoke_t> constexpr auto operator()(invoke_t &&invokable) const noexcept
    {
        // static_assert(std::is_integral_v<std::remove_cvref_t<invoke_t>>);
        // static_assert(std::is_invocable_v<invoke_t, std::string_view>);

        if constexpr (std::is_invocable_v<invoke_t, std::string_view>)
        {
            return action_invoke<invoke_t, letter_parser_ignore_case>(std::forward<invoke_t>(invokable),
                                                                      letter_parser_ignore_case{});
        }
        else
        //(detail::ConstructorTakesTwoIterators<invoke_t, std::forward_iterator, std::forward_iterator>)
        {
            return action_array_like(std::forward<invoke_t>(invokable), letter_parser_ignore_case{});
        }
    }
};
}