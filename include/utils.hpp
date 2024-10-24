#ifndef TAPESORT_SRC_UTILS_HPP
#define TAPESORT_SRC_UTILS_HPP

#include <cctype>
#include <cinttypes>
#include <iostream>

namespace tape_sort
{
    struct TapeParams
    {
        size_t begin;
        size_t end;
        size_t size;
    };

    enum TapePosition
    {
        kStart = 0,
        kEnd = 2
    };
}

#endif // TAPESORT_SRC_UTILS_HPP