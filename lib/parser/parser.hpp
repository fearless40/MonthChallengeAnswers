#pragma once
#include "attributes.hpp"
#include "concepts.hpp"
#include "dsl.hpp"
#include "follows.hpp"
#include "joins.hpp"
#include <algorithm>
#include <concepts>

namespace parser {

namespace cs = parser::concepts;

template <cs::is_parser p1_t, cs::is_parser p2_t>
auto operator>>(p1_t &&p1, p2_t &&p2) {
  return parser::follows<p1_t, p2_t>(std::forward<p1_t>(p1),
                                     std::forward<p2_t>(p2));
};

template <cs::is_parser p1_t, cs::is_parser p2_t>
auto operator+(p1_t &&p1, p2_t &&p2) {
  return parser::joins<p1_t, p2_t>(p1, p2);
}

template <typename stream_t, typename parser_t, typename wskip_t>
bool parse(stream_t &&stream, parser_t parser,
           wskip_t ws = dsl::white_space_not_endl)

{
  auto begin = std::begin(stream);
  auto end = std::end(stream);
  return parser.parse(begin, end, ws);
}

} // namespace parser
