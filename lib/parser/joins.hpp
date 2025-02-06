#pragma once

namespace parser
{

/**
 * @brief Different then follows in its semantic meaning. The callback is executed at the end of both parses and no
 * whitespace parsing
 *
 */
template <typename p1_t, typename p2_t> struct joins
{
    using parser_tag = attributes::parser_tag;
    using attributes = attributes::Attributes<>;
    using type = joins<p1_t, p2_t>;

    p1_t p1;
    p2_t p2;

    joins(p1_t parser1, p2_t parser2) : p1(parser1), p2(parser2) {};

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        if (p1.parse(begin, end, ws))
        {
            return p2.parse(begin, end, ws);
        }
        return false;
    }

    template <typename invoke_t> constexpr auto operator()(invoke_t &&invokable) const noexcept
    {
        if constexpr (std::is_invocable_v<invoke_t, std::string_view>)
        {
            return action_invoke{std::forward<invoke_t>(invokable), std::move(*this)};
        }
        else
        //(detail::ConstructorTakesTwoIterators<invoke_t, std::forward_iterator, std::forward_iterator>)
        {
            return action_array_like{std::forward<invoke_t>(invokable), std::move(*this)};
        }
    }
};
} // namespace parser