#include <cinttypes>
#include <fstream>
#include <iostream>

#include "config_reader.hpp"
#include "driver.hpp"
#include "tape.hpp"

int main(int argv, char** argc) {
  if (4 < argv || argv < 3) {
    std::cerr << "Wrong number of arguments!" << std::endl;
    return 1;
  }

  std::string config_file_path{"config/config.json"};
  if (argv == 4) config_file_path = argc[3];

  try {
    ConfigReader config(config_file_path);

    tape_sort::Tape input_tape(argc[1]);
    tape_sort::Tape output_tape(argc[2], std::ios::trunc);

    tape_sort::TapeFactory factory;
    tape_sort::Driver tape_driver(config, factory);

    tape_driver.Sort(output_tape, input_tape);

    tape_sort::DriverPerfStats::PrintStat();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}