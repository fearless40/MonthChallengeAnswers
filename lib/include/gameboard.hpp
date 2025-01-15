#pragma once

#include <RowCol.hpp>
#include <optional>
#include <vector>

class GameBoard
{
    std::size_t rows;
    std::size_t cols;
    std::vector<std::int16_t> values;

    std::optional<std::int16_t> at(const RowCol &pos)
    {
        auto index = to_index(pos);
        // std::cout << std::format("Input: {}:{} index: {} / {}", pos.col, pos.row, index, values.size()) << '\n';
        if (index >= values.size())
        {
            return {};
        }
        return values[index];
    }

        constexpr std::size_t to_index(const RowCol` &pos)
    {
        return pos.row * cols + pos.col;
    }
};