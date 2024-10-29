#ifndef TAPESORT_SRC_GENERATOR_HPP
#define TAPESORT_SRC_GENERATOR_HPP

#include <cinttypes>
#include <iostream>
#include <random>

#include "config_reader.hpp"

namespace tape_sort
{
    class TapeGenerator
    {
    public:
        TapeGenerator(const std::string&);
        ~TapeGenerator();

        void Generate(uint64_t);

    private:
        std::ofstream stream_;
    };
} // namespace tape_sort

#endif // TAPESORT_SRC_GENERATOR_HPP