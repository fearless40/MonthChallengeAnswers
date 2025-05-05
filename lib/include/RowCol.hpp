#pragma once
#include <compare>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>

namespace battleship {

namespace detail {
template <typename Tag, typename SizeT = std::uint16_t>
struct GameBoardPosition {
  using type = SizeT;
  explicit GameBoardPosition(SizeT sz) : size(sz) {}
  SizeT size;
  auto operator<=>(GameBoardPosition<Tag> const &other) const = default;
  bool operator==(GameBoardPosition<Tag> const &other) const = default;
};

namespace Tags {
struct row;
struct col;
}; // namespace Tags
} // namespace detail

// Strong typing to prevent errors
using Row = detail::GameBoardPosition<detail::Tags::row>;
using Col = detail::GameBoardPosition<detail::Tags::col>;

struct RowCol {
  Row row;
  Col col;
  RowCol() : row(0), col(0) {};
  explicit RowCol(Row r, Col c) : row(r), col(c) {};

  auto operator<=>(const RowCol &other) const = default;

  std::string as_base26_fmt() const;

  std::string as_colrow_fmt() const;

  static RowCol random(Row maxRow, Col maxCol);

  static RowCol from_string(std::string_view const);
};

} // namespace battleship
