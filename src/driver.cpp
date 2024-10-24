#include "driver.hpp"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

namespace tape_sort
{
    using AdvancedTapeQueue = std::priority_queue<
        std::pair<Tape, TapeParams>,
        std::vector<std::pair<Tape, TapeParams>>,
        tape_sort::Driver::CompareTapeBySize
    >;
}

bool tape_sort::Driver::CompareTapeBySize::operator()(const std::pair<Tape, TapeParams>& lhs, const std::pair<Tape, TapeParams>& rhs)
{
    if (lhs.second.size != rhs.second.size)
        return lhs.second.size > rhs.second.size;

    return lhs.second.begin > rhs.second.begin;
}

tape_sort::Driver::Driver(const DriverParams& driver_params) : driver_params_(driver_params)
{
};

tape_sort::Driver::~Driver() = default;

int32_t tape_sort::Driver::get(Tape& tape)
{
    ++drive_perf_stats_.reading_count_;
    sleep_for(milliseconds(driver_params_.item_read_delay));
    return tape.get();
}

void tape_sort::Driver::set(Tape& tape, int32_t value)
{
    ++drive_perf_stats_.writing_count_;
    sleep_for(milliseconds(driver_params_.item_write_delay));
    return tape.set(value);
}

void tape_sort::Driver::Rewind(Tape& tape, TapePosition pos)
{
    ++drive_perf_stats_.rewind_count_;
    sleep_for(milliseconds(driver_params_.tape_rewind_delay));
    return tape.Rewind(pos);
}

void tape_sort::Driver::StepForward(Tape& tape)
{
    ++drive_perf_stats_.step_shift_count_;
    sleep_for(milliseconds(driver_params_.step_shift_delay));
    return tape.StepForward();
}

std::string tape_sort::Driver::TapeTitleCreate(TapeParams& params)
{
    return std::string(
        driver_params_.temp_file_dir_name +
        std::to_string(params.begin) + '-' +
        std::to_string(params.end) + '_' +
        std::to_string(params.size) +
        driver_params_.temp_file_format
    );
}

void tape_sort::Driver::CopyTape(Tape& output, Tape& input, size_t num) {
    while (num)
    {
        set(output, get(input));
        StepForward(input);
        StepForward(output);
        --num;
    }
}

void tape_sort::Driver::Sort(Tape& output, Tape& input)
{
    std::filesystem::create_directory(driver_params_.temp_file_dir_name);
    AdvancedTapeQueue tape_storage{ std::move(CreateTapePriorityQueue(input)) };

    if (!tape_storage.size())
        return;

    if (tape_storage.size() == 1)
    {
        auto pair{ std::move(const_cast<std::pair<Tape, TapeParams>&>(tape_storage.top())) };
        tape_storage.pop();

        CopyTape(output, pair.first, pair.second.size);
        return;
    }

    while (tape_storage.size() > 2)
        MergeSort(tape_storage);

    auto first_pair{ std::move(const_cast<std::pair<Tape, TapeParams>&>(tape_storage.top())) };
    tape_storage.pop();

    auto second_pair{ std::move(const_cast<std::pair<Tape, TapeParams>&>(tape_storage.top())) };
    tape_storage.pop();

    Merge(output, first_pair, second_pair);
}

tape_sort::AdvancedTapeQueue tape_sort::Driver::CreateTapePriorityQueue(Tape& input)
{
    size_t input_size{ 0 };
    AdvancedTapeQueue tape_storage;

    while (input.good())
    {
        size_t curr_tape_start{ input_size };
        std::vector<int32_t> storage;
        size_t remaining_size{ driver_params_.max_elem_number };
        storage.reserve(remaining_size);

        while (remaining_size && input.good())
        {
            storage.push_back(get(input));
            StepForward(input);
            --remaining_size;
        }

        std::sort(storage.begin(), storage.end());

        input_size += driver_params_.max_elem_number - remaining_size;
        TapeParams curr_tape_params{ curr_tape_start, input_size - 1, input_size - curr_tape_start };
        std::string curr_file_path{ std::move(TapeTitleCreate(curr_tape_params)) };
        Tape curr(curr_file_path, std::ios::trunc);

        for (int32_t i : storage)
        {
            set(curr, i);
            StepForward(curr);
        }

        Rewind(curr, TapePosition::kStart);

        tape_storage.push({ std::move(curr), curr_tape_params });
    }

    return tape_storage;
}

void tape_sort::Driver::MergeSort(AdvancedTapeQueue& queue)
{
    auto first_pair{ std::move(const_cast<std::pair<Tape, TapeParams>&>(queue.top())) };
    queue.pop();

    auto second_pair{ std::move(const_cast<std::pair<Tape, TapeParams>&>(queue.top())) };
    queue.pop();

    size_t start{ std::min(first_pair.second.begin, second_pair.second.begin) };
    size_t end{ std::max(first_pair.second.end, second_pair.second.end) };
    size_t size{ first_pair.second.size + second_pair.second.size };

    TapeParams result_tape_params{ start, end, size };
    std::string curr_file_path{ std::move(TapeTitleCreate(result_tape_params)) };
    Tape result_tape(curr_file_path, std::ios::trunc);

    Merge(result_tape, first_pair, second_pair);
    Rewind(result_tape, TapePosition::kStart);

    queue.push({ std::move(result_tape), result_tape_params });
}

void tape_sort::Driver::Merge(Tape& result, std::pair<Tape, TapeParams>& left, std::pair<Tape, TapeParams>& right)
{
    Tape& lhs{ left.first };
    Tape& rhs{ right.first };

    size_t left_tape_size{ left.second.size };
    size_t right_tape_size{ right.second.size };

    int32_t left_value{ get(lhs) };
    int32_t right_value{ get(rhs) };

    while (left_tape_size && right_tape_size)
    {
        if (left_value <= right_value)
        {
            set(result, left_value);
            StepForward(lhs);
            left_value = get(lhs);
            --left_tape_size;
        }
        else
        {
            set(result, right_value);
            StepForward(rhs);
            right_value = get(rhs);
            --right_tape_size;
        }
        StepForward(result);
    }

    while (left_tape_size)
    {
        set(result, get(lhs));
        StepForward(lhs);
        StepForward(result);
        --left_tape_size;
    }

    while (right_tape_size)
    {
        set(result, get(rhs));
        StepForward(rhs);
        StepForward(result);
        --right_tape_size;
    }
}