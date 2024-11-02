#include "driver.hpp"

tape_sort::DriverParams::DriverParams(const ConfigReader& config)
    : item_write_delay(config.get<uint32_t>("tape_delays", "item_write")),
      item_read_delay(config.get<uint32_t>("tape_delays", "item_read")),
      tape_rewind_delay(config.get<uint32_t>("tape_delays", "rewind")),
      step_shift_delay(config.get<uint32_t>("tape_delays", "step_shift")) {
  size_t max_bytes_for_elems{
      config.get<size_t>("driver_settings", "max_bytes_for_elements")};
  elem_num = max_bytes_for_elems / sizeof(uint32_t);
  if (elem_num <= 1) throw MaxTempTapeSizeException();
};

tape_sort::DriverParams::DriverParams(size_t elem_number,
                                      uint32_t item_write_delay,
                                      uint32_t item_read_delay,
                                      uint32_t tape_rewind_delay,
                                      uint32_t step_shift_delay)
    : elem_num(elem_number),
      item_write_delay(item_write_delay),
      item_read_delay(item_read_delay),
      tape_rewind_delay(tape_rewind_delay),
      step_shift_delay(step_shift_delay) {
  if (elem_number <= 1) throw MaxTempTapeSizeException();
};

tape_sort::DriverParams::~DriverParams() = default;