#include "commandline.hpp"

int main(int argc, char const * argv[])
{
    auto opt = parse(argc, argv);
    if( !opt )
        return -1;

    return 0;


    std::string filename;
    
    bool runAutomated = false;




/*
    auto board = load_from_file(filename);
    if (!board)
    {
        error("File could not be loaded. Quitting.");
        return -1;
    }

    if (runAutomated)
    {
        non_interactive_mode(board.value(), guesses);
        std::flush(std::cout);
        return 0;
    }
    else
    {
        interactive_mode(board.value(), filename);
        return 0;
    }

    */
}