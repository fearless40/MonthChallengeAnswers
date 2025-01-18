#pragma once

#include <RowCol.hpp>
#include <optional>
#include <vector>

class GameBoard
{
    size_t m_rows;
    size_t m_cols;
    std::vector<std::int16_t> m_values;

    constexpr bool is_valid_index(const RowCol &pos) const
    {
        return (pos.row >= 0 && pos.row <= m_rows) && (pos.col >= 0 && pos.col <= m_cols);
    }

    int16_t operator[](const RowCol &pos)
    {
        return m_values[to_index(pos)];
    }

    std::optional<std::int16_t> at(const RowCol &pos)
    {
        if (!is_valid_index(pos))
            return {};

        auto index = to_index(pos);
        return m_values[index];
    }

    constexpr std::size_t to_index(const RowCol &pos)
    {
        return pos.row * m_cols + pos.col;
    }
};