#pragma once
#include <collisions.hpp>
#include <string>
#include <vector>

enum class Orientation
{
    Horizontal,
    Vertical
};

struct Ship
{
    AABB location;
    uint32_t shiplength;
    constexpr Orientation orientation() const noexcept
    {
        if ((location.x2 - location.x) > (location.y2 - location.y))
            return Orientation::Horizontal;

        return Orientation::Vertical;
    }
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
    std::vector<Player> players;

    constexpr static bool valid_row_col(uint16_t row, uint16_t col)
    {
        return row >= 0 && row < std::numeric_limits<uint16_t>::max() && col >= 0 &&
               col < std::numeric_limits<uint16_t>::max();
    }
};