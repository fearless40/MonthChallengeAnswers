#pragma once
namespace parser
{
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

} // namespace parser