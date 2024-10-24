#include "driver.hpp"

tape_sort::DriverParams::DriverParams(const ConfigReader& config) :
    max_elem_number(config.get<size_t>("driver_settings", "max_elem_number")),
    item_write_delay(config.get<uint32_t>("tape_delays", "item_write")),
    item_read_delay(config.get<uint32_t>("tape_delays", "item_read")),
    tape_rewind_delay(config.get<uint32_t>("tape_delays", "rewind")),
    step_shift_delay(config.get<uint32_t>("tape_delays", "step_shift")),
    temp_file_dir_name(config.get<std::string>("driver_settings", "temp_file_dir_name")),
    temp_file_format(config.get<std::string>("driver_settings", "temp_file_format"))
{
};

tape_sort::DriverParams::DriverParams(
    size_t max_elem_number,
    uint32_t item_write_delay,
    uint32_t item_read_delay,
    uint32_t tape_rewind_delay,
    uint32_t step_shift_delay,
    std::string temp_file_dir_name,
    std::string temp_file_format
) :
    max_elem_number(max_elem_number),
    item_write_delay(item_write_delay),
    item_read_delay(item_read_delay),
    tape_rewind_delay(tape_rewind_delay),
    step_shift_delay(step_shift_delay),
    temp_file_dir_name(temp_file_dir_name),
    temp_file_format(temp_file_format)
{
};

tape_sort::DriverParams::~DriverParams() = default;