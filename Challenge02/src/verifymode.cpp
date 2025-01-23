#include "verifymode.hpp"
#include "error.hpp"
#include "fileloader.hpp"
#include "iostream"
#include <algorithm>

int run_command_mode_verify(const commandline::ProgramOptions &opt)
{
    ErrorReport errs;
    Game g = load_from_file(opt.filename, errs);
    if (errs)
    {
        std::ranges::for_each(errs.errors,
                              [](const std::string &str) { std::cout << std::format("ERROR: {} \n", str); });
    }

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

    return 0;
}