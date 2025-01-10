#include "commandline.hpp"
#include "terminal.hpp"
#include <iostream>
#include <ranges>

void print_use()
{
    std::puts(
        R"(
Usage:
    challenge01v2 --load filename [--verbose(-v)] [--guess (query1 query2 query3 ...)]
    --load (-l) specify the file name to load
    [--verbose] put extra details into the output
    [--nomem]   do not store the file into memory. Just read and and spit out answers
    [--guess(-g)] (guess1 guess2) enter the queries to check the results of the file. 
    )");
}

std::optional<CommandLineOptions> parse(int argc, char const *argv[])
{
    CommandLineOptions opt;
    int r = 50, g = 250, b = 200;
    for (int i : std::views::iota(1, 60))
    {
        // char buf[2] = {c, '\0'};
        /*char buff[32];
        char *next = Term::to_chars(buff, &buff[32], 255);
        *next = '\0';*/

        // std::cout << buff << '\n';
        //  std::cout << Term::color_rgb(++r, ++g, ++b).c_str() << c;

        std::fputs(Term::color_rgb_back(r += 1, g -= 1, b -= 2).c_str(), stdout);
        std::fputs(" ", stdout);
        // std::fputs(Term::color_rgb_back(255 - (r += 20), 255 - (g -= 15), 255 - (b -= 21)).c_str(), stdout);
        // std::fputs(buf, stdout);
    }

    std::fputs(Term::term_fcolor(Term::Color::reset), stdout);

    if (argc == 1)
    {
        print_use();
        return {};
    }

    for (int cmdIndex = 1; cmdIndex != argc; ++cmdIndex)
    {
        std::string_view arg{argv[cmdIndex]};

        if (arg == "--load" || arg == "-l")
        {
            if (cmdIndex + 1 > argc)
            {
                std::fputs(Term::color_rgb(100, 200, 224).c_str(), stdout);
                std::puts("Missing --load option. --load also requires a file name.");
                return {};
            }
            arg = argv[++cmdIndex];
            opt.fileToParse = arg;
            continue;
        }

        if (arg == "--verbose" || arg == "-v")
        {
            opt.verbose = true;
            continue;
        }

        if (arg == "--nomem")
        {
            opt.nomem = true;
            continue;
        }

        if (arg == "--guess" || arg == "-g")
        {
            opt.interactive = false;
            opt.queries.reserve(argc - (cmdIndex + 1));
            for (int guessIndex = cmdIndex + 1; guessIndex < argc; ++guessIndex)
                opt.queries.emplace_back(argv[guessIndex]);

            if (opt.queries.empty())
            {
                std::puts("Missing queries (guess).");
                print_use();
                return {};
            }
            break;
        }
    }

    if (opt.fileToParse.empty())
    {
        // std::fputs( Term::color_rgb(100,200,224).c_str(), stdout );
        std::puts("Missing --load option. --load also requires a file name.");
        print_use();
        return {};
    }

    return opt;
}