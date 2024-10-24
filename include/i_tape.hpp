#ifndef TAPESORT_SRC_ITAPE_HPP
#define TAPESORT_SRC_ITAPE_HPP

#include <cinttypes>
#include <fstream>

#include "utils.hpp"

namespace tape_sort
{
    class ITape
    {
    public:
        ITape() = default;
        ITape(const ITape&) = delete;
        ITape& operator=(const ITape&) = delete;
        ITape(ITape&&) noexcept = default;
        ITape& operator=(ITape&&) noexcept = default;
        virtual ~ITape() = default;

        virtual int32_t get() = 0;
        virtual void set(int32_t) = 0;
        virtual void Rewind(TapePosition) = 0;
        virtual void StepForward() = 0;
        virtual bool good() = 0;
    };
}

#endif // TAPESORT_SRC_ITAPE_HPP