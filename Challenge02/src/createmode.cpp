#include <algorithm>
#include <createmode.hpp>
#include <format>
#include <game.hpp>
#include <iostream>

std::optional<Ship> parse_ship(std::string_view str)
{
    return {};
}

int run_command_mode_create(const commandline::ProgramOptions &opt)
{
    ErrorReport report;
    Game game;
    game.rows = opt.rows;
    game.cols = opt.cols;
    game.minShipSize = opt.ship_smallest;
    game.maxShipSize = opt.ship_largest;

    size_t playerId = 0;
    for (auto &cPlayer : opt.players)
    {
        Player p;
        p.name = cPlayer.name;
        p.id = (uint32_t)playerId++;
        if (cPlayer.isRandom)
        {
            p.generate_random_ships(game, report);
        }
        game.players.push_back(p);
    }

    print_game(game);
    game.report_game_is_valid(report);
    print_player(game, game.players.front());

    if (report)
    {
        std::ranges::for_each(report.errors,
                              [](const std::string &str) { std::cout << std::format("ERROR: {} \n", str); });
    }

    return 0;
}