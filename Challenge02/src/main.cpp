
#include "commandline.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    auto opt = commandline::parse(argc, argv);

    std::cout << "Read in: \n";
    std::cout << "Mode: " << (int)opt.mode << '\n';
    std::cout << "Filename: " << opt.filename << '\n';
    std::cout << "Rows: " << opt.rows << '\n';
    std::cout << "Cols: " << opt.cols << '\n';
    std::cout << "Smallest Ship: " << opt.ship_smallest << '\n';
    std::cout << "Largest Ship: " << opt.ship_largest << '\n';
    std::cout << "Numer of players: " << opt.players.size() << '\n';

    for (auto &player : opt.players)
    {
        std::cout << "\tPlayer Name: " << player.name << '\n';
        std::cout << "\tIs random: " << player.isRandom << '\n' << '\t';
        for (auto &placements : player.placements)
        {
            std::cout << placements << " ";
        }
    }
    std::cout << '\n';
}