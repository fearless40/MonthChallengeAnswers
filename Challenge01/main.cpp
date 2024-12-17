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

using std::operator""sv;

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
                 { return std::isdigit(static_cast<unsigned char>(c)) || c == '-'; });

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

struct ReadComma
{
    std::string data;
    int line{0};
    friend std::istream &operator>>(std::istream &stream, ReadComma &reader)
    {
        stream >> reader.data;
        reader.line = reader.line + 1;
        return stream;
    }
};

struct FileData
{
    std::size_t rows;
    std::size_t cols;
    std::vector<std::int16_t> values;
};

struct Position
{
    std::size_t row;
    std::size_t col;
};

std::optional<Position> parse_excel_fmt(std::string_view value)
{
    auto pos = value.find_first_of("0123456789");
    if (pos == std::string_view::npos)
    {
        error("Cant find numbers in excel format");
        return {};
    }

    auto letters = value.substr(0, pos);
    // Convert all letters to lowercase
    auto toLower = [](auto l)
    { return std::tolower(static_cast<unsigned char>(l)); };

    std::size_t numberLetters = letters.size();
    std::size_t col = 0;
    for (const auto l : letters | std::views::transform(toLower))
    {
        col += (l - 'a') + ((numberLetters - 1) * 26);
        --numberLetters;
    }

    auto numbers = value.substr(pos);
    auto numb = parse_number_positive(numbers);
    if (std::holds_alternative<int>(numb))
    {
        return {
            {static_cast<std::size_t>(std::get<int>(numb)),
             static_cast<std::size_t>(col)}};
    }
    return {};
}

std::optional<Position>
parse_comma_fmt(std::string_view value)
{
    auto pos = value.find_first_of(",");
    auto colText = value.substr(0, pos);
    auto rowText = value.substr(pos + 1);

    if (colText.size() == 0 || rowText.size() == 0)
    {
        error("Invalid col row comma format");
        return {};
    }

    auto varCol = parse_number_positive(colText);
    auto varRow = parse_number_positive(rowText);

    if (std::holds_alternative<int>(varCol) && std::holds_alternative<int>(varRow))
    {
        return {{static_cast<std::size_t>(std::get<int>(varRow)),
                 static_cast<std::size_t>(std::get<int>(varCol))}};
    }
    return {};
}

std::optional<Position>
parse_row_col_format(std::string_view value)
{
    // Check the format
    auto pos = value.find_first_of(",");
    if (pos != std::string_view::npos)
    {
        return parse_comma_fmt(value);
    }

    // else do excel reading
    return parse_excel_fmt(value);
}

std::optional<FileData> load_from_file(std::filesystem::path filename)
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

    if (rowCount == 0 || rowCount > 65535)
    {
        error("Rowcount too large or 0");
        return {};
    }

    if (colCount == 0 || colCount > 65535)
    {
        error("Colcount too large or 0");
        return {};
    }

    std::cout << std::format("Row Count: {}\nCol Count: {}\n", rowCount, colCount);

    std::vector<std::int16_t> data;
    std::vector<std::string> test;
    auto stringView = std::views::istream<std::string>(file);

    std::size_t nbrRows = 0;
    for (auto &l : stringView)
    {
        ++nbrRows;
        std::stringstream ss{l};
        std::string temp;
        std::size_t nbrCols = 0;
        while (std::getline(ss, temp, ','))
        {
            auto ret = parse_number(temp);
            if (std::holds_alternative<int>(ret))
            {
                auto val = std::get<int>(ret);
                if (val < -32768 || val > 32767)
                {
                    error(std::format("Data value to large at: row:{} col:{}", nbrRows, nbrCols));
                    return {};
                }
                data.push_back(static_cast<std::int16_t>(val));
                ++nbrCols;
            }
            else
            {
                error(std::get<const std::string_view>(ret));
                std::cout << std::format("Found on row: {}, col {}", nbrRows, nbrCols) << '\n';
                break;
            }
        }

        if (nbrCols != colCount)
        {
            error(std::format("Invalid number of cols on row: {}", nbrRows));
            return {};
        }
    }
    if (nbrRows != rowCount)
    {
        error("Not enough rows in the data.");
        return {};
    }

    return {{rowCount, colCount, std::move(data)}};
}

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

    auto optBoard = load_from_file(filename);
    if (!optBoard)
        return 0;

    auto val = *optBoard;

    for (auto &item : guesses)
    {
        auto opt = parse_row_col_format(item);
        if (opt)
        {
            std::cout << std::format("Col: {}  Row: {}", opt.value().col, opt.value().row) << '\n';
        }
    }

    std::ranges::copy(val.values, std::ostream_iterator<int>(std::cout, " | "));
}