#ifndef TAPESORT_INCLUDE_TAPE_HPP_
#define TAPESORT_INCLUDE_TAPE_HPP_

#include <cinttypes>
#include <fstream>

#include "config_reader.hpp"
#include "filesystem"
#include "i_tape.hpp"
#include "tape_generator.hpp"

namespace tape_sort {
class Tape : public ITape {
 public:
  Tape(const std::string&,
       std::ios_base::openmode = std::ios::in | std::ios::out);

  Tape(Tape&&) noexcept;
  Tape& operator=(Tape&&) noexcept;
  ~Tape();

  int32_t get() override;
  void set(int32_t) override;
  void Rewind(TapePosition) override;
  void StepForward() override;
  bool good() override;

 private:
  std::fstream stream_;
};

class TapeFactory : public ITapeFactory {
 public:
  TapeFactory(const ConfigReader& config);
  TapeFactory(std::string temp_file_dir_name = "tmp/",
              std::string temp_file_format = ".txt");
  std::shared_ptr<ITape> Create(const std::string&) override;

 private:
  const std::string kTempFileDirName;
  const std::string kTempFileFormat;
};
}  // namespace tape_sort

#endif  // TAPESORT_INCLUDE_TAPE_HPP_