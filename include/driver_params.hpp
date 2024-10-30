#ifndef TAPESORT_INCLUDE_DRIVERPARAMS_HPP_
#define TAPESORT_INCLUDE_DRIVERPARAMS_HPP_

#include <cinttypes>

#include "config_reader.hpp"

namespace tape_sort {
class DriverParams {
 public:
  DriverParams(const ConfigReader& config);

  DriverParams(size_t max_elem_number = 2048, uint32_t item_write_delay = 0,
               uint32_t item_read_delay = 0, uint32_t tape_rewind_delay = 0,
               uint32_t step_shift_delay = 0,
               std::string temp_file_dir_name = "tmp/",
               std::string temp_file_format = ".txt");

  ~DriverParams();

  size_t max_elem_number;
  uint32_t item_write_delay;
  uint32_t item_read_delay;
  uint32_t tape_rewind_delay;
  uint32_t step_shift_delay;
  std::string temp_file_dir_name;
  std::string temp_file_format;
};
}  // namespace tape_sort

#endif  // TAPESORT_INCLUDE_DRIVERPARAMS_HPP_