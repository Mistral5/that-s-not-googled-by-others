#include "tape.hpp"

tape_sort::Tape::Tape(const std::string& file_name,
                      std::ios_base::openmode mode)
    : stream_(file_name, mode | std::ios::in | std::ios::out) {
  if (!stream_.is_open()) throw AssociatedFileException{file_name};
}

tape_sort::Tape::Tape(Tape&&) noexcept = default;

tape_sort::Tape& tape_sort::Tape::operator=(Tape&&) noexcept = default;

tape_sort::Tape::~Tape() noexcept = default;

int32_t tape_sort::Tape::get() {
  const auto init_pos{stream_.tellg()};
  int32_t value;
  stream_ >> value;
  stream_.seekg(init_pos, std::ios::beg);
  return value;
}

void tape_sort::Tape::set(int32_t value) {
  auto init_pos{stream_.tellp()};
  stream_ << value << ' ';
  stream_.seekp(init_pos, std::ios::beg);
}

void tape_sort::Tape::Rewind(TapePosition pos) {
  stream_.clear();
  stream_.seekg(pos);
  stream_.seekp(pos);
}

void tape_sort::Tape::StepForward() {
  int32_t value;
  stream_ >> value;
  stream_.seekp(1 + stream_.tellg(), std::ios::beg);
}

bool tape_sort::Tape::good() {
  auto symbol = stream_.peek();
  return stream_.good() && symbol != '\n' && symbol != EOF;
}

tape_sort::TapeFactory::TapeFactory(const ConfigReader& config)
    : kTempFileDirName(config.get<std::string>("temp_file_dir_name")),
      kTempFileFormat(config.get<std::string>("temp_file_format")) {
  std::filesystem::create_directory(kTempFileDirName);

  if (!std::filesystem::exists(kTempFileDirName))
    throw TempDirNotExistException{kTempFileDirName};
}

tape_sort::TapeFactory::TapeFactory(std::string temp_file_dir_name,
                                    std::string temp_file_format)
    : kTempFileDirName(temp_file_dir_name), kTempFileFormat(temp_file_format) {
  std::filesystem::create_directory(kTempFileDirName);

  if (!std::filesystem::exists(kTempFileDirName))
    throw TempDirNotExistException{kTempFileDirName};
}

std::shared_ptr<tape_sort::ITape> tape_sort::TapeFactory::Create(
    const std::string& file_name) {
  std::string tape_name(kTempFileDirName + file_name + kTempFileFormat);
  std::ofstream stream(tape_name, std::ios::trunc);

  if (!stream.is_open()) throw AssociatedFileException{file_name};

  return std::shared_ptr<ITape>(
      std::make_shared<Tape>(tape_name, std::ios::trunc));
}