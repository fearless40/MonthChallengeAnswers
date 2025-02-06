#pragma once
#include <tuple>

namespace parser::attributes
{

struct parser_tag
{
};

struct ws_never
{
};

struct ws_always
{
};

template <class... T> using Attributes = std::tuple<T...>;

template <typename T>
concept has_attributes = T::attributes;

template <typename T, typename Tuple> struct contains_type
{
    static constexpr bool value = false;
};

template <typename T, typename Head, typename... Tail> struct contains_type<T, std::tuple<Head, Tail...>>
{
    static constexpr bool value = std::is_same_v<T, Head> || contains_type<T, std::tuple<Tail...>>::value;
};

template <typename T, typename Tuple> struct has_type;

template <typename T, typename... Us> struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...>
{
};

template <typename Attrib_t>
concept never_calls_ws = has_type<ws_never, Attrib_t>::value;

} // namespace parser::attributes
