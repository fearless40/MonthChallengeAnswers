#pragma once

#include "int.hpp"
#include "letters.hpp"

namespace parser::dsl
{
template <typename T> inline constexpr parser::int_parser<T> int_parser = parser::int_parser<T>{};

constexpr parser::letter_parser_ignore_case letters_ignore_case = parser::letter_parser_ignore_case{};
} // namespace parser::dsl
