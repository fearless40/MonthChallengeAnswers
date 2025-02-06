#pragma once

#include "attributes.hpp"
#include "empty.hpp"
#include "matchers.hpp"

namespace parser
{

struct white_space_not_endl
{

    using parser_tag = attributes::parser_tag;
    using attributes = attributes::Attributes<attributes::ws_always>;

    template <class begin_t, class end_t, class wskip_t = empty>
    constexpr bool parse(begin_t &begin, end_t &end, wskip_t skip = empty{}) const noexcept
    {
        return matchers::match_white_space_not_endl(begin, end);
    }
};

} // namespace parser