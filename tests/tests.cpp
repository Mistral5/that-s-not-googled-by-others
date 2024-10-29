#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <vector>

#include "config_reader.hpp"
#include "driver.hpp"
#include "driver_params.hpp"
#include "i_tape.hpp"
#include "tape.hpp"
#include "tape_generator.hpp"

class TestClass {
 public:
  static std::vector<int32_t> ReadTape(std::string& tape_name) {
    int32_t curr{0};
    std::ifstream input_tape(tape_name);
    std::vector<int32_t> input_data;
    while (input_tape >> curr) input_data.push_back(curr);
    input_tape.close();
    return input_data;
  }

  static void WriteTape(std::vector<int32_t>& data, std::string& tape_name) {
    std::ofstream output_tape(tape_name);
    for (int32_t i : data) output_tape << i << ' ';
    output_tape.close();
  }
};

TEST(tape_sort, default_case)  // num_test > max_elem_number
{
  // Arrange
  size_t num_test{10000};
  std::string input_tape_name{"test_" + std::to_string(num_test) + ".txt"};
  std::string output_tape_name{"test_" + std::to_string(num_test) + "_out.txt"};

  {
    tape_sort::TapeGenerator gen(input_tape_name);
    gen.Generate(num_test);
  }

  std::vector<int32_t> input_data(
      std::move(TestClass::ReadTape(input_tape_name)));
  std::sort(input_data.begin(), input_data.end());
  // Act
  {
    tape_sort::Tape input_tape(input_tape_name);
    tape_sort::Tape output_tape(output_tape_name,
                                std::ios::in | std::ios::out | std::ios::trunc);

    tape_sort::TapeFactory factory;
    tape_sort::Driver tape_driver({}, factory);

    tape_driver.Sort(output_tape, input_tape);
  }

  // Assert
  std::vector<int32_t> output_data(
      std::move(TestClass::ReadTape(output_tape_name)));

  ASSERT_EQ(input_data, output_data);
}

TEST(tape_sort, corner_case)  // INT32_MAX, INT32_MIN
{
  // Arrange
  std::vector<int32_t> input_data{INT32_MAX, INT32_MIN, 1, 0, -1};
  size_t num_test{input_data.size()};
  std::string input_tape_name{"test_" + std::to_string(num_test) + ".txt"};
  std::string output_tape_name{"test_" + std::to_string(num_test) + "_out.txt"};

  TestClass::WriteTape(input_data, input_tape_name);
  std::sort(input_data.begin(), input_data.end());

  // Act
  {
    tape_sort::Tape input_tape(input_tape_name);
    tape_sort::Tape output_tape(output_tape_name, std::ios::trunc);

    tape_sort::TapeFactory factory;
    tape_sort::Driver tape_driver({}, factory);
    tape_driver.Sort(output_tape, input_tape);
  }

  // Assert
  std::vector<int32_t> output_data(
      std::move(TestClass::ReadTape(output_tape_name)));

  ASSERT_EQ(input_data, output_data);
}

TEST(tape_sort, short_tape_case)  // num_test < max_elem_number
{
  // Arrange
  tape_sort::DriverParams temp;
  size_t num_test{temp.max_elem_number >> 1 + temp.max_elem_number % 2};
  std::string input_tape_name{"test_" + std::to_string(num_test) + ".txt"};
  std::string output_tape_name{"test_" + std::to_string(num_test) + "_out.txt"};
  {
    tape_sort::TapeGenerator gen(input_tape_name);
    gen.Generate(num_test);
  }
  std::vector<int32_t> input_data(
      std::move(TestClass::ReadTape(input_tape_name)));
  std::sort(input_data.begin(), input_data.end());

  // Act
  {
    tape_sort::Tape input_tape(input_tape_name);
    tape_sort::Tape output_tape(output_tape_name, std::ios::trunc);

    tape_sort::TapeFactory factory;
    tape_sort::Driver tape_driver({}, factory);
    tape_driver.Sort(output_tape, input_tape);
  }

  // Assert
  std::vector<int32_t> output_data(
      std::move(TestClass::ReadTape(output_tape_name)));

  ASSERT_EQ(input_data, output_data);
}

TEST(tape_sort, empty_tape_case) {
  // Arrange
  std::vector<int32_t> input_data{};
  size_t num_test{input_data.size()};
  std::string input_tape_name{"test_" + std::to_string(num_test) + ".txt"};
  std::string output_tape_name{"test_" + std::to_string(num_test) + "_out.txt"};

  TestClass::WriteTape(input_data, input_tape_name);
  std::sort(input_data.begin(), input_data.end());

  // Act
  {
    tape_sort::Tape input_tape(input_tape_name);
    tape_sort::Tape output_tape(output_tape_name, std::ios::trunc);

    tape_sort::TapeFactory factory;
    tape_sort::Driver tape_driver({}, factory);
    tape_driver.Sort(output_tape, input_tape);
  }

  // Assert
  std::vector<int32_t> output_data(
      std::move(TestClass::ReadTape(output_tape_name)));

  ASSERT_EQ(input_data, output_data);
}

TEST(config_reader, fields_search) {
  std::string config_file_path{"config/config.json"};
  std::ifstream stream(config_file_path);
  ASSERT_EQ(stream.is_open(), true);
  ASSERT_EQ(nlohmann::json::accept(stream), true);

  stream.seekg(std::ios::beg);
  nlohmann::json config_json = nlohmann::json::parse(stream);
  ConfigReader config(config_file_path);

  uint32_t expected{
      config_json.at("tape_delays").at("item_write").get<uint32_t>()};
  uint32_t actual{config.get<uint32_t>("tape_delays", "item_write")};
  EXPECT_EQ(expected, actual);

  std::string expected_message{};
  std::string actual_message{};

  try {
    config_json.at("aboba").get<uint32_t>();
  } catch (const std::exception& e) {
    expected_message = e.what();
  }

  try {
    config.get<uint32_t>("aboba");
  } catch (const std::exception& e) {
    actual_message = e.what();
  }

  EXPECT_EQ(expected_message, actual_message);
}