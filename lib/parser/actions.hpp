#pragma once

#include "baseconv.hpp"
#include <algorithm>
#include <functional>
#include <type_traits>

namespace parser {
template <typename invokable_t, typename parser_t> struct action_invoke {
  using wrapped_parser_t = std::remove_cvref_t<parser_t>;
  using parser_tag = std::remove_cvref_t<parser_t>::parser_tag;
  using attributes = wrapped_parser_t::attributes;

  invokable_t invokable;
  parser_t parser;

  action_invoke(invokable_t &&invoke, parser_t &&parser_wrapper)
      : invokable(invoke), parser(parser_wrapper) {}

  template <class begin_t, class end_t, class wskip_t>
  constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept {

    begin_t start = begin;
    if (parser.parse(begin, end, ws)) {
      std::invoke(invokable, std::string_view{start, begin});
      return true;
    }
    return false;
  }
};
template <typename int_t, typename parser_t> struct action_int_ref {
  int_t &value;
  parser_t parser;

  using wrapped_parser_t = std::remove_cvref_t<parser_t>;
  using parser_tag = parser_t::parser_tag;
  using attributes = wrapped_parser_t::attributes;

  action_int_ref(int_t &val, parser_t &&parser_wrapper)
      : value(val), parser(parser_wrapper) {}

  template <class begin_t, class end_t, class wskip_t>
  constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept {

    begin_t start = begin;
    if (parser.parse(begin, end, ws)) {
      value = base10::from_string({start, begin});
      return true;
    }
    return false;
  }
};

template <typename array_t, typename parser_t> struct action_array_like {
  array_t &value;
  parser_t parser;

  using wrapped_parser_t = std::remove_cvref_t<parser_t>;
  using parser_tag = parser_t::parser_tag;
  using attributes = wrapped_parser_t::attributes;

  action_array_like(array_t &val, parser_t parser_wrapper)
      : value(val), parser(std::forward<parser_t>(parser_wrapper)) {}

  template <class begin_t, class end_t, class wskip_t>
  constexpr bool parse(begin_t &begin, end_t &end, wskip_t ws) const noexcept {
    begin_t start = begin;
    if (parser.parse(begin, end, ws)) {
      value = array_t{start, begin};
      return true;
    }
    return false;
  }
};
} // namespace parser
