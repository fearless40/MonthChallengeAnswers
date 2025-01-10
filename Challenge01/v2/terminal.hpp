#pragma once
#include <format>


namespace Term{
#define ESC(value) "\x1B["#value
#define CMD(value) "\x1B["#value"m"
#define COLOR(name,value) constexpr const char * name = "\x1B["#value"m"

namespace Raw {
constexpr const char * escape = "\x1B";
constexpr const char * reset = ESC(0);
constexpr const char * bold = ESC(1);
constexpr const char * italic = ESC(3);
constexpr const char * underline = ESC(4);
};

namespace Codes {
    constexpr const char * backcolor = "4";
    constexpr const char * forecolor = "3";
}


enum class Text {
    reset = 0,
    bold = 1,
    italic = 3,
    underline = 4
};



namespace Forecolor {
    COLOR(black, 30);
    COLOR(red,31);
    COLOR(green,32);
    COLOR(yellow,33);
    COLOR(blue,	34);
    COLOR(magenta,	35);
    COLOR(cyan,	36);
    COLOR(white,	37);
    COLOR(defaultc,	39);
    COLOR(reset,	0);
};

enum class Color {
    black = 0,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white,
    defaultc,
    reset
};

std::string color_rgb( int r, int g, int b) {
    return std::format("{}[38;2;{};{};{}m",Raw::escape,r,g,b); 
}



constexpr const char * term_fcolor(Color c) {
    

    switch( c ){
        case Color::black:
        return Forecolor::black;
        case Color::blue:
        return Forecolor::blue;
        case Color::red:
        return Forecolor::red;
        case Color::green:
        return Forecolor::green;
        case Color::magenta:
        return Forecolor::magenta;
        case Color::cyan:
        return Forecolor::cyan;
        case Color::white:
        return Forecolor::white;
        case Color::defaultc:
        return Forecolor::defaultc;
        case Color::reset:
        return Forecolor::reset;
    };
    return Forecolor::reset;
    
    //return std::format("{}[{}{}",Raw::escape, foreColor ? 3 : 4,static_cast<int>(c));
}


}