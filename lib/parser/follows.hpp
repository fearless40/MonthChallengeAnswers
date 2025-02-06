#pragma once

template <typename p1_t, typename p2_t> struct follows
{
    p1_t p1;
    p2_t p2;

    using parser_tag = detail::parser_tag;

    constexpr follows(p1_t &parse1, p2_t &parse2) : p1(parse1), p2(parse2) {};

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        if constexpr (detail::attributes::never_calls_ws<typename p1_t::attributes>)
        {
            ws.parse(begin, end, ws);
        }

        if (p1.parse(begin, end, ws))
        {
            static_assert(detail::attributes::never_calls_ws<typename p2_t::attributes> == true);
            if constexpr (detail::attributes::never_calls_ws<typename p2_t::attributes>)
            {
                ws.parse(begin, end, ws);
            }
            return p2.parse(begin, end, ws);
        }
        return false;
    }
};
