#include "config_reader.hpp"

ConfigReader::ConfigReader(const std::string& file_name) {
  std::ifstream stream(file_name, std::ios::in);

  if (!stream.is_open()) throw AssociatedFileException{file_name};

  config_json_ = nlohmann::json::parse(stream);
};

ConfigReader::~ConfigReader() = default;
