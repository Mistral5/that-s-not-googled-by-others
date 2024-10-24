#ifndef TAPESORT_SRC_DRIVER_HPP
#define TAPESORT_SRC_DRIVER_HPP

#include <chrono>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <thread>
#include <queue>
#include <utility>

#include "config_reader.hpp"
#include "driver_params.hpp"
#include "driver_perf_stats.hpp"
#include "tape.hpp"
#include "utils.hpp"

namespace tape_sort
{
    class Driver
    {
    public:
        Driver(const DriverParams&);
        ~Driver();

        int32_t get(Tape&);
        void set(Tape&, int32_t);
        void Rewind(Tape&, TapePosition);
        void StepForward(Tape&);

        void Sort(Tape&, Tape&);
        void CopyTape(Tape&, Tape&, size_t);

        struct CompareTapeBySize {
            bool operator()(const std::pair<Tape, TapeParams>&, const std::pair<Tape, TapeParams>&);
        };

    private:
        std::priority_queue<
            std::pair<Tape, TapeParams>,
            std::vector<std::pair<Tape, TapeParams>>,
            CompareTapeBySize
        > CreateTapePriorityQueue(Tape&);

        void MergeSort(std::priority_queue<std::pair<Tape, TapeParams>,
            std::vector<std::pair<Tape, TapeParams>>, CompareTapeBySize>&);
        void Merge(Tape&, std::pair<Tape, TapeParams>&, std::pair<Tape, TapeParams>&);
        std::string TapeTitleCreate(TapeParams&);

        const DriverParams driver_params_;
        DriverPerfStats drive_perf_stats_;
    };
}

#endif // TAPESORT_SRC_DRIVER_HPP