#include "RowCol.hpp"
#include <array>
#include <atomic>
#include <string>
#include <vector>

void high_speed_file_read(std::vector<RowCol> query, std::string filename)
{
    // Read 64k at a time
    std::array<char *, 5> buffers;
    std::atomic<int> nextBuff;
    std::atomic<int> currentBuf;

    for (std::size_t i = 0; i < 5; ++i)
    {
        buffers[i] = new char[64000];
    }
}
