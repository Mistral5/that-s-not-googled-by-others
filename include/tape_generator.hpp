#ifndef TAPESORT_INCLUDE_TAPEGENERATOR_HPP_
#define TAPESORT_INCLUDE_TAPEGENERATOR_HPP_

#include <cinttypes>
#include <iostream>
#include <random>

#include "config_reader.hpp"

namespace tape_sort {
class TapeGenerator {
 public:
  TapeGenerator(const std::string&);
  ~TapeGenerator();

  void Generate(uint64_t);

 private:
  std::ofstream stream_;
};
}  // namespace tape_sort

#endif  // TAPESORT_INCLUDE_TAPEGENERATOR_HPP_