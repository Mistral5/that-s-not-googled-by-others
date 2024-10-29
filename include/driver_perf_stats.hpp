#ifndef TAPESORT_SRC_DRIVER_PERF_STATS_HPP
#define TAPESORT_SRC_DRIVER_PERF_STATS_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

namespace tape_sort
{
    class DriverPerfStats
    {
    public:
        DriverPerfStats();
        ~DriverPerfStats();

        static void PrintStat();

        static inline uint64_t reading_count_{};
        static inline uint64_t writing_count_{};
        static inline uint64_t rewind_count_{};
        static inline uint64_t step_shift_count_{};
    };
} // namespace tape_sort

#endif // TAPESORT_SRC_DRIVER_PERF_STATS_HPP