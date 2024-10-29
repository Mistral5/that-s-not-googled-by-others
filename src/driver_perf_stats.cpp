#include "driver_perf_stats.hpp"

tape_sort::DriverPerfStats::DriverPerfStats() = default;

tape_sort::DriverPerfStats::~DriverPerfStats() = default;

void tape_sort::DriverPerfStats::PrintStat() {
  std::cout << "Reading count: " << reading_count_
            << "\nWriting count: " << writing_count_
            << "\nRewind count: " << rewind_count_
            << "\nStep shift count: " << step_shift_count_ << std::endl;
}