#pragma once

#include <RowCol.hpp>
#include <optional>
#include <vector>

namespace battleship {
/**
 * @description A 2D array of values defined by [0,nbrRows), [0,nbrCols)
 * @note Not inclusive of the Row and Col
 */

template <typename T> class Array2D {
  Row m_rows;
  Col m_cols;
  std::vector<T> m_values;

  Array2D() = delete;
  explicit Array2D(Row nbrRows, Col nbrCols)
      : m_rows(nbrRows), m_cols(nbrCols) {
    m_values.reserve(m_rows.size * m_cols.size);
  }

  constexpr bool is_valid_index(const RowCol &pos) const {
    return (pos.row.size >= 0 && pos.row.size <= m_rows.size) &&
           (pos.col.size >= 0 && pos.col.size <= m_cols.size);
  }

  constexpr T &operator[](const RowCol &pos) { return m_values[to_index(pos)]; }

  constexpr void set(const RowCol &pos, T &value) {
    if (auto mem_opt = at(pos); mem_opt) {
      mem_opt.value() = value;
    }
  }

  constexpr std::optional<T &> at(const RowCol &pos) {
    if (!is_valid_index(pos))
      return {};

    auto index = to_index(pos);
    return m_values[index];
  }

  constexpr std::size_t to_index(const RowCol &pos) {
    return pos.row.size * m_cols.size + pos.col.size;
  }
};
} // namespace battleship
