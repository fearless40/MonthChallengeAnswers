#include "game.hpp"
#include "RandomUtil.hpp"
#include "collisions.hpp"
#include <algorithm>
#include <format>
#include <iostream>
#include <terminal.hpp>

std::optional<Ship> Player::ship_at_position(uint16_t row, uint16_t col) const
{
    for (auto &ship : ships)
    {
        int y = (int)row;
        int x = (int)col;
        if (ship.location.contains_point(x, y))
            return ship;
    }
    return {};
}

bool Player::generate_random_ships(Game const &game, ErrorReport &report)
{

    for (uint32_t ship_id = game.minShipSize; ship_id < game.maxShipSize + 1; ++ship_id)
    {

        auto const size_vector = ships.size();
        for (size_t count_attempts = 0; count_attempts < 1000; ++count_attempts)
        {
            Orientation layout = random::coin_flip() == true ? Orientation::Horizontal : Orientation::Vertical;
            uint16_t width = (layout == Orientation::Vertical ? 0 : ship_id - 1);
            uint16_t height = (layout == Orientation::Horizontal ? 0 : ship_id - 1);

            int row = random::between(0, game.rows - 1 - height);
            int col = random::between(0, game.cols - 1 - width);
            AABB position{col, row, col + width, row + height};
            if (!check_if_position_collides(position))
            {
                ships.emplace_back(ship_id, position);
                // report(std::format("Player {} generated: {} after {} tries", name, ship_id, count_attempts));
                break;
            }
            else
            {
                ++count_attempts;
            }
        }
        if (ships.size() == size_vector)
        {
            report(std::format("Player {} could not generate: {} after {} tries", name, ship_id, 1000));
            return false;
        }
    }
    return true;
}

bool Game::report_game_is_valid(ErrorReport &report) const noexcept
{
    for (auto &player : players)
    {
        for (auto shipit = player.ships.begin(); shipit != player.ships.end(); ++shipit)
        {
            auto &ship = *shipit;
            if (!ship.is_valid())
            {
                report(std::format("Ship Id: {} is invalid.", ship.shiplength));
                return false;
            }

            if (ship.shiplength > maxShipSize || ship.shiplength < minShipSize)
            {
                report(std::format("Player: {}'s Ship Id: {} is larger than game specified.", player.name, ship.id()));
                return false;
            }

            if (std::ranges::any_of(shipit + 1, player.ships.end(), [&](auto &next_ship) {
                    /*std::cout << std::format("{}'s comparing {} to {} collides = {}\n", player.name, ship.id(),
                                             next_ship.id(), aabb_collision(ship.location, next_ship.location));*/

                    if (aabb_collision(ship.location, next_ship.location))
                    {
                        report(std::format("{}'s ships collide:  {} collides with {}.", player.name, ship.id(),
                                           next_ship.id()));
                        return true;
                    }

                    return false;
                }))
            {

                return false;
            }
        }
    }

    return true;
}

void print_game(Game const &g)
{
    std::cout << std::format("Game:\nRows: {}\nCols: {}\nShips: {} to {}\n", g.rows, g.cols, g.minShipSize,
                             g.maxShipSize);
    for (auto &p : g.players)
    {

        std::cout << "Player: " << p.name << '\n';
        for (auto &s : p.ships)
        {
            std::cout << '\t' << "Ship ID: " << s.shiplength << '\n';
            std::cout << '\t' << "Ship valid: " << s.is_valid() << '\n';
            std::cout << '\t' << "Ship orientation: " << (s.orientation() == Orientation::Horizontal ? 'H' : 'V')
                      << '\n';
            std::cout << '\t'
                      << std::format("Ship Location: x: {} y: {} x2: {} y2: {}", s.location.x, s.location.y,
                                     s.location.x2, s.location.y2)
                      << '\n';
        }
    }
}

void print_player(Game const &g, Player const &p)
{
    std::cout << '\n';
    for (int row = -1; row < g.rows; ++row)
    {
        for (int col = -1; col < g.cols; ++col)
        {
            if (row == -1)
            {
                // Print headers
                if (col == -1 && row == -1)
                    std::cout << "\t";
                else
                    std::cout << std::format("{}\t", (char)('A' + col));
            }
            else
            {
                if (col == -1)
                {
                    std::cout << row + 1 << '\t';
                }
                else
                {
                    auto opt = p.ship_at_position(row, col);
                    if (opt)
                    {
                        std::cout << Term::fcolor(Term::Color::yellow) << opt.value().id()
                                  << Term::fcolor(Term::Color::reset);
                    }
                    else
                        std::cout << 0;
                    std::cout << '\t';
                }
            }
        }
        std::cout << '\n';
    }
}