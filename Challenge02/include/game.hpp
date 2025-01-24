#pragma once
#include "RowCol.hpp"
#include <collisions.hpp>
#include <string>
#include <vector>
#include "error.hpp"

enum class Orientation
{
    Horizontal,
    Vertical
};

const int UNSET = -1;

struct Ship
{
    uint32_t shiplength;
    AABB location{UNSET, UNSET, UNSET, UNSET};
    constexpr Orientation orientation() const noexcept
    {
        if ((location.x2 - location.x) > (location.y2 - location.y))
            return Orientation::Horizontal;

        return Orientation::Vertical;
    }

    constexpr int row_size() const noexcept
    {
        return location.x2 - location.x;
    }

    constexpr int col_size() const noexcept
    {
        return location.y2 - location.y;
    }

    constexpr bool is_valid() const noexcept
    {
        return (row_size() == 0 && col_size() == shiplength - 1) || (row_size() == shiplength - 1 && col_size() == 0);
    }

    constexpr uint32_t id() const noexcept { return shiplength;}
};

struct Player
{
    uint32_t id;
    std::string name;
    std::vector<Ship> ships;
};

struct Game
{
    uint16_t rows;
    uint16_t cols;
    uint32_t minShipSize;
    uint32_t maxShipSize;
    std::vector<Player> players;

    constexpr static bool valid_row_col(uint16_t row, uint16_t col)
    {
        return row >= 0 && row < std::numeric_limits<uint16_t>::max() && col >= 0 &&
               col < std::numeric_limits<uint16_t>::max();
    }

    constexpr bool valid_pos(RowCol pos) const noexcept
    {
        return (pos.row >= 0 and pos.row <= rows) and (pos.col >= 0 and pos.col <= cols);
    }

    bool report_game_is_valid(ErrorReport & report) const noexcept;
    
};