#pragma once

#include "attributes.hpp"
#include <concepts>

namespace parser::concepts
{
template <class T>
concept CharIterator = requires(T a) {
    { *a } -> std::convertible_to<char>;
};

template <class T>
concept is_parser = std::same_as<typename std::remove_cvref_t<T>::parser_tag, parser::attributes::parser_tag>;

template <class T, class Begin_t, class End_t>
concept ConstructorTakesTwoIterators = requires(T a, Begin_t b, End_t e) {
    { T{b, e} } -> std::same_as<T>;
};

template <typename T>
concept has_attributes = T::attributes;

template <typename T, typename Tuple> struct has_type;

template <typename T, typename... Us> struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...>
{
};

template <typename AttribTupple_t>
concept never_calls_ws = has_type<parser::attributes::ws_never, AttribTupple_t>::value;

} // namespace parser::concepts