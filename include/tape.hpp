#ifndef TAPESORT_SRC_TAPE_IMPL_HPP
#define TAPESORT_SRC_TAPE_IMPL_HPP

#include <cinttypes>
#include <fstream>

#include "config_reader.hpp"
#include "i_tape.hpp"
#include "utils.hpp"

namespace tape_sort
{
    class Tape : public ITape
    {
    public:
        Tape(const std::string&, std::ios_base::openmode = std::ios::in | std::ios::out);
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
}

#endif // TAPESORT_SRC_TAPE_IMPL_HPP