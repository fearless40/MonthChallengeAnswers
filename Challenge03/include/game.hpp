#pragma once
#include "RowCol.hpp"
#include <compare>
#include <iterator>

namespace Battleship {
struct ShipSize {
  std::size_t size;

  auto operator<=>(ShipSize const &other) const = default;
  bool operator==(ShipSize const &other) const = default;
};

using ShipID = ShipSize;

namespace detail {
template <typename Tag> struct GameBoardPosition {
  std::size_t size;
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

struct BoardDescription {
  Row rows;
  Col cols;
  ShipSize min;
  ShipSize max;

  constexpr std::size_t begin_rows() const { return 0; }
  constexpr std::size_t end_rows() const { return rows.size; }
  constexpr std::size_t begin_cols() const { return 0; }
  constexpr std::size_t end_cols() const { return cols.size; }
};

using ShipPosition = RowCol;
// struct ShipPosition {
//   Row row;
//   Col col;
//
//   constexpr bool is_valid(const BoardDescription &desc) {
//     return row.size >= desc.begin_rows() && row.size < desc.end_rows() &&
//            col.size >= desc.begin_cols() && col.size < desc.end_cols();
//   }
// };

} // namespace Battleship
