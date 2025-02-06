#pragma once

#include <concepts>

namespace parser::concepts
{
template <class T>
concept CharIterator = requires(T a) {
    { *a } -> std::convertible_to<char>;
};

template <class T>
concept is_parser = std::same_as<typename std::remove_cvref_t<T>::parser_tag, parser_tag>;

template <class T, class Begin_t, class End_t>
concept ConstructorTakesTwoIterators = requires(T a, Begin_t b, End_t e) {
    { T{b, e} } -> std::same_as<T>;
};

} // namespace parser::concepts