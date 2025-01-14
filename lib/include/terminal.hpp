#pragma once
#include <array>
#include <format>

namespace Term
{
#define ESC(value) "\x1B[" #value
#define CMD(value) "\x1B[" #value "m"
#define COLOR(name, value) constexpr const char *name = "\x1B[" #value "m"

namespace Raw
{
constexpr const char *escape = "\x1B";
constexpr const char *reset = ESC(0);
constexpr const char *bold = ESC(1);
constexpr const char *italic = ESC(3);
constexpr const char *underline = ESC(4);
}; // namespace Raw

namespace Codes
{
constexpr const char *backcolor = "4";
constexpr const char *forecolor = "3";
} // namespace Codes

enum class Text
{
    reset = 0,
    bold = 1,
    italic = 3,
    underline = 4
};

namespace Forecolor
{
COLOR(black, 30);
COLOR(red, 31);
COLOR(green, 32);
COLOR(yellow, 33);
COLOR(blue, 34);
COLOR(magenta, 35);
COLOR(cyan, 36);
COLOR(white, 37);
COLOR(defaultc, 39);
COLOR(reset, 0);

std::array<const char *, 10> colors{black, red, green, yellow, blue, magenta, cyan, white, defaultc, reset};
}; // namespace Forecolor

enum class Color
{
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

constexpr char *to_chars(char *start, char *end, unsigned char value)
{
    if (&start[2] >= end)
        return end;

    int nbrChars = value < 10 ? 1 : value < 100 ? 2 : 3;
    int i = nbrChars;
    do
    {
        start[--i] = '0' + (value % 10);
        value /= 10;
    } while (value != 0);
    return &start[nbrChars];
}

namespace detail
{
constexpr const char *forecolor_rgb_escape_sequence()
{
    return "\x1b[38;2;";
}

constexpr const char *backcolor_rgb_escape_sequence()
{
    return "\x1b[48;2;";
}

} // namespace detail

constexpr std::string color_rgb_fore_back(int r, int g, int b, bool fore = true)
{
    char buff[32];
    const char *escape = fore ? detail::forecolor_rgb_escape_sequence() : detail::backcolor_rgb_escape_sequence();
    for (int i = 0; i < 8; ++i)
    {
        buff[i] = escape[i];
    }

    // char buff[32] = fore == true ? detail::forecolor_rgb_escape_sequence() : detail::backcolor_rgb_escape_sequence();
    char *next = &buff[7];

    next = to_chars(next, &buff[32], r);
    *next = ';';
    next = to_chars(++next, &buff[32], g);
    *next = ';';
    next = to_chars(++next, &buff[32], b);
    *next = 'm';
    *(++next) = '\0';

    return std::string(buff);
}

constexpr std::string fcolor_rgb(int r, int g, int b)
{
    return color_rgb_fore_back(r, g, b, true);
}

constexpr std::string bcolor_rgb(int r, int g, int b)
{
    return color_rgb_fore_back(r, g, b, false);
}

constexpr const char *fcolor(Color c)
{
    return Forecolor::colors[static_cast<size_t>(c)];
}

} // namespace Term