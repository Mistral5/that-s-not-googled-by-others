#ifndef TAPESORT_INCLUDE_DRIVER_HPP_
#define TAPESORT_INCLUDE_DRIVER_HPP_

#include <chrono>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "config_reader.hpp"
#include "driver_params.hpp"
#include "driver_perf_stats.hpp"
#include "i_tape.hpp"

namespace tape_sort {
struct TapeParams {
  size_t begin;
  size_t end;
  size_t size;
};

class Driver {
 public:
  Driver(const DriverParams&, ITapeFactory&);
  ~Driver();

  int32_t get(ITape&);
  void set(ITape&, int32_t);
  void Rewind(ITape&, TapePosition);
  void StepForward(ITape&);

  void Sort(ITape&, ITape&);
  void ToTape(ITape&, ITape&, size_t);
  void ToTape(ITape&, std::vector<int32_t>&);

  using AdvancedTape =
      std::pair<std::shared_ptr<tape_sort::ITape>, tape_sort::TapeParams>;

  struct CompareTapeBySize {
    bool operator()(const AdvancedTape&, const AdvancedTape&);
  };

  using AdvancedTapeQueue =
      std::priority_queue<AdvancedTape, std::vector<AdvancedTape>,
                          tape_sort::Driver::CompareTapeBySize>;

 private:
  AdvancedTapeQueue CreateTapePriorityQueue(ITape&);
  void MergeSort(AdvancedTapeQueue&);

  void Merge(ITape&, AdvancedTape&, AdvancedTape&);
  std::string TapeTitleCreate(TapeParams&);

  const DriverParams driver_params_;
  DriverPerfStats drive_perf_stats_;
  ITapeFactory& factory_;
};
}  // namespace tape_sort

#endif  // TAPESORT_INCLUDE_DRIVER_HPP_