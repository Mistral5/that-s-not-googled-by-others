#include "tape_generator.hpp"

tape_sort::TapeGenerator::TapeGenerator(const std::string& file_name) : stream_(file_name, std::ios::out)
{
    if (!stream_.is_open())
        throw AssociatedFileException{ file_name };
}

tape_sort::TapeGenerator::~TapeGenerator() = default;

void tape_sort::TapeGenerator::Generate(uint64_t num_elems)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(INT32_MIN, INT32_MAX);

    for (uint64_t i = 0; i != num_elems; ++i)
        stream_ << dist(gen) << ' ';
}
