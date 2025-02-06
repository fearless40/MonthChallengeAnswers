#pragma once

#include "int.hpp"
#include "letters.hpp"
#include "singlechar.hpp"
#include "whitespace.hpp"

namespace parser::dsl
{
template <typename T> inline constexpr parser::int_parser<T> int_parser = parser::int_parser<T>{};

constexpr parser::letter_parser_ignore_case letters_ignore_case = parser::letter_parser_ignore_case{};

constexpr parser::white_space_not_endl white_space_not_endl = parser::white_space_not_endl{};

template <char l> inline constexpr parser::single_char<l> single_char = parser::single_char<l>{};
} // namespace parser::dsl
