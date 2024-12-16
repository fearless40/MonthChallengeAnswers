#include <iostream>
#include <string>
#include <vector>
#include <format>
#include "..\external\clipp\include\clipp.h"

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
}