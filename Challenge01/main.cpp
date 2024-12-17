#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <optional>
#include <filesystem>
#include <fstream>
#include <variant>
#include <ranges>
#include <algorithm>
#include <charconv>
#include "..\external\clipp\include\clipp.h"

using namespace std::literals;

void error(const std::string_view errorMsg)
{
    std::cout << "ERROR: " << errorMsg << '\n';
}

std::variant<int, const std::string_view> parse_number(const std::string_view string)
{
    std::string val;

    auto foundLetter = std::find_if(string.begin(), string.end(), [](auto c)
                                    { return std::isalpha(static_cast<unsigned char>(c)); });

    if (foundLetter != string.end())
    {
        return "Letters found in input."sv;
    }

    std::copy_if(string.begin(), string.end(), std::back_inserter(val), [](auto c)
                 { return std::isdigit(static_cast<unsigned char>(c)); });

    int number;
    auto result = std::from_chars(val.data(), val.data() + val.size(), number);
    if (result.ec == std::errc())
    {
        return number;
    }
    else if (result.ec == std::errc::invalid_argument)
    {
        return "Invalid argument to parser";
    }
    else if (result.ec == std::errc::result_out_of_range)
    {
        return "Parsing value is too large";
    }
}

std::variant<int, const std::string_view> parse_number_positive(const std::string_view string)
{
    // Ensure there is no negtive number
    auto pos = string.find('-');
    if (pos != std::string_view::npos)
    {
        return "No negative numbers allowed.";
    }

    return parse_number(string);
}

class GameBoard
{
public:
    static std::optional<std::unique_ptr<GameBoard>> load_from_file(std::filesystem::path filename)
    {
        if (!std::filesystem::exists(filename))
        {
            error("Filename not found.");
            std::cout << "Filename: " << filename << '\n';
        }

        std::ifstream file{filename};
        char buff[255];

        unsigned int rowCount = 0;
        unsigned int colCount = 0;

        {
            file.getline(buff, 256);
            auto rowParse = parse_number_positive(buff);
            if (std::holds_alternative<const std::string_view>(rowParse))
            {
                error(std::get<const std::string_view>(rowParse));
            }
            else
            {
                rowCount = std::get<int>(rowParse);
            }
        }

        {
            file.getline(buff, 256);
            auto colParse = parse_number_positive(buff);
            if (std::holds_alternative<const std::string_view>(colParse))
            {
                error(std::get<const std::string_view>(colParse));
            }
            else
            {
                colCount = std::get<int>(colParse);
            }
        }

        std::cout << std::format("Row Count: {}\nCol Count: {}\n", rowCount, colCount);

        return {};
    }
};

int main(int argc, char *argv[])
{
    using namespace clipp;
    // using cout = std::cout;

    std::string filename;
    std::vector<std::string> guesses;

    auto cli = (required("--load") & value("file", filename),
                required("--guess") & values("guesses", guesses));

    auto result = parse(argc, argv, cli);

    if (not result)
    {
        std::cout << "Usage:\n"
                  << usage_lines(cli, "challenge01");
        return -1;
    }

    std::cout << "Filename: " << filename << '\n';
    std::cout << std::format("Found: {}", guesses.size()) << "\n";

    GameBoard::load_from_file(filename);
}