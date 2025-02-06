#pragma once

#include "attributes.hpp"
#include "concepts.hpp"

namespace parser
{
template <typename p1_t, typename p2_t> struct follows
{
    p1_t p1;
    p2_t p2;

    using parser_tag = attributes::parser_tag;
    using attributes = attributes::Attributes<attributes::ws_always>;
    using parser1_t = std::remove_cvref_t<p1_t>;
    using parser2_t = std::remove_cvref_t<p2_t>;

    constexpr follows(p1_t parse1, p2_t parse2) : p1(parse1), p2(parse2) {};

    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        if constexpr (concepts::never_calls_ws<typename parser1_t::attributes>)
        {
            ws.parse(begin, end, ws);
        }

        if (p1.parse(begin, end, ws))
        {

            if constexpr (concepts::never_calls_ws<typename parser2_t::attributes>)
            {
                ws.parse(begin, end, ws);
            }
            return p2.parse(begin, end, ws);
        }
        return false;
    }
};
} // namespace parser