#include "fileloader.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

const size_t BUFFER_SIZE = 4096;

void skip_ws(std::ifstream &file)
{
    char c = file.peek();
    while (c == ' ' || c == '\t' || c == '\v')
    {
        file.get();
        c = file.peek();
    }
}

std::vector<Ship> read_ship_data(std::ifstream &file, ErrorReport &error, int rowSize, int colSize)
{
    int rowCount = 0;
    int colCount = 0;
    std::vector<Ship> ships;

    while (file && rowCount < rowSize && colCount < colSize)
    {
        int mapValue;
        file >> mapValue;

        if (file.fail())
        {
            error("Unable to read value in file data.");
            return {};
        }

        if (mapValue > 0)
        {
            auto it = std::ranges::find(ships, static_cast<uint32_t>(mapValue), &Ship::shiplength);
            if (it != ships.end())
            {
                auto &loc = (*it).location;
                loc.x2 = colCount;
                loc.y2 = rowCount;
            }
            else
            {
                // Create a ship
                ships.push_back(Ship{(uint32_t)mapValue, {colCount, rowCount, UNSET, UNSET}});
            }
        }

        skip_ws(file);

        if (char c = file.peek(); c == ',')
        {
            ++colCount;
            file.get();
        }
        else if (c == '\n' || c == '\r')
        {
            if (colCount != colSize - 1 || colCount == 0)
            {
                error(std::format("Found only this many cols: {} out of {}", colCount + 1, colSize));
                return {};
            }

            ++rowCount;
            colCount = 0;
            file >> std::ws;
        }
    }

    if (rowCount != rowSize)
    {
        error(std::format("Found only this many rows: {} out of {}", rowCount + 1, rowSize));
        return {};
    }

    return ships;
}

Game load_from_file(std::filesystem::path filename, ErrorReport &error)
{
    if (!std::filesystem::exists(filename))
    {
        error(std::format("File not found: {}", "hmm"));
        return {};
    }

    std::ifstream file{filename};

    unsigned int rowCount = 0;
    unsigned int colCount = 0;
    size_t nbrPlayers;

    file >> rowCount >> colCount >> nbrPlayers;
    if (!file)
    {
        error("Unable to read rowcount, colcount or nbrplayers");
        return {};
    }

    Game ret{(std::uint16_t)rowCount, (std::uint16_t)colCount, 2, 5};
    ret.players.reserve(nbrPlayers);

    for (size_t playerId = 0; playerId < nbrPlayers; ++playerId)
    {
        std::string playerName;
        file >> playerName;
        if (!file)
        {
            error("Unable to read player name");
            return {};
        }

        ret.players.emplace_back((uint32_t)playerId, playerName);
        auto &player = ret.players.back();

        player.ships = read_ship_data(file, error, ret.rows, ret.cols);
    }

        return ret;
}
