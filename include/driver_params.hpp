#ifndef TAPESORT_INCLUDE_DRIVERPARAMS_HPP_
#define TAPESORT_INCLUDE_DRIVERPARAMS_HPP_

#include <cinttypes>

#include "config_reader.hpp"
#include "exceptions.hpp"

namespace tape_sort {
class DriverParams {
 public:
  DriverParams(const ConfigReader& config);

  DriverParams(size_t elem_num = 512, uint32_t item_write_delay = 0,
               uint32_t item_read_delay = 0, uint32_t tape_rewind_delay = 0,
               uint32_t step_shift_delay = 0);

  ~DriverParams();

  size_t elem_num;
  uint32_t item_write_delay;
  uint32_t item_read_delay;
  uint32_t tape_rewind_delay;
  uint32_t step_shift_delay;
};
}  // namespace tape_sort

#endif  // TAPESORT_INCLUDE_DRIVERPARAMS_HPP_