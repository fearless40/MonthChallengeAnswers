#pragma once

#include "attributes.hpp"

namespace parser
{
struct empty
{
    using parser_tag = attributes::parser_tag;
    using attributes = attributes::Attributes<attributes::ws_never>;
    template <class begin_t, class end_t, class wskip_t>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept
    {
        return true;
    }
};

} // namespace parser