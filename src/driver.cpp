#include "driver.hpp"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

bool tape_sort::Driver::CompareTapeBySize::operator()(const AdvancedTape& lhs, const AdvancedTape& rhs)
{
    if (lhs.second.size != rhs.second.size)
        return lhs.second.size > rhs.second.size;

    return lhs.second.begin > rhs.second.begin;
}

tape_sort::Driver::Driver(const DriverParams& driver_params, ITapeFactory& factory) :
    driver_params_(driver_params),
    factory_(factory)
{
};

tape_sort::Driver::~Driver() = default;

int32_t tape_sort::Driver::get(ITape& tape)
{
    ++drive_perf_stats_.reading_count_;
    sleep_for(milliseconds(driver_params_.item_read_delay));
    return tape.get();
}

void tape_sort::Driver::set(ITape& tape, int32_t value)
{
    ++drive_perf_stats_.writing_count_;
    sleep_for(milliseconds(driver_params_.item_write_delay));
    return tape.set(value);
}

void tape_sort::Driver::Rewind(ITape& tape, TapePosition pos)
{
    ++drive_perf_stats_.rewind_count_;
    sleep_for(milliseconds(driver_params_.tape_rewind_delay));
    return tape.Rewind(pos);
}

void tape_sort::Driver::StepForward(ITape& tape)
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

void tape_sort::Driver::ToTape(ITape& output, ITape& input, size_t num) {
    while (--num)
    {
        set(output, get(input));
        StepForward(input);
        StepForward(output);
    }
    set(output, get(input));
}

void tape_sort::Driver::ToTape(ITape& output, std::vector<int32_t>& input)
{
    for (auto el{ input.begin() }; el != std::prev(input.end()); ++el)
    {
        set(output, *el);
        StepForward(output);
    }
    set(output, *std::prev(input.end()));
}

void tape_sort::Driver::Sort(ITape& output, ITape& input)
{
    std::filesystem::create_directory(driver_params_.temp_file_dir_name);

    AdvancedTapeQueue tape_storage{ CreateTapePriorityQueue(input) };

    if (!tape_storage.size())
        return;

    if (tape_storage.size() == 1)
    {
        AdvancedTape pair{ const_cast<AdvancedTape&>(tape_storage.top()) };
        tape_storage.pop();
        ToTape(output, *pair.first, pair.second.size);
        return;
    }

    while (tape_storage.size() > 2)
        MergeSort(tape_storage);

    AdvancedTape first_pair{ const_cast<AdvancedTape&>(tape_storage.top()) };
    tape_storage.pop();

    AdvancedTape second_pair{ const_cast<AdvancedTape&>(tape_storage.top()) };
    tape_storage.pop();

    Merge(output, first_pair, second_pair);
}

tape_sort::Driver::AdvancedTapeQueue tape_sort::Driver::CreateTapePriorityQueue(ITape& input)
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

        std::shared_ptr<ITape> curr_tape{ factory_.Create(curr_file_path, std::ios::trunc) };

        ToTape(*curr_tape, storage);
        Rewind(*curr_tape, TapePosition::kStart);

        tape_storage.push({ curr_tape, curr_tape_params });
    }

    return tape_storage;
}

void tape_sort::Driver::MergeSort(AdvancedTapeQueue& queue)
{
    AdvancedTape first_pair{ const_cast<AdvancedTape&>(queue.top()) };
    queue.pop();

    AdvancedTape second_pair{ const_cast<AdvancedTape&>(queue.top()) };
    queue.pop();

    size_t start{ std::min(first_pair.second.begin, second_pair.second.begin) };
    size_t end{ std::max(first_pair.second.end, second_pair.second.end) };
    size_t size{ first_pair.second.size + second_pair.second.size };

    TapeParams result_tape_params{ start, end, size };
    std::string curr_file_path{ std::move(TapeTitleCreate(result_tape_params)) };

    std::shared_ptr<ITape> result_tape{ factory_.Create(curr_file_path, std::ios::trunc) };

    Merge(*result_tape, first_pair, second_pair);
    Rewind(*result_tape, TapePosition::kStart);

    queue.push({ result_tape, result_tape_params });
}

void tape_sort::Driver::Merge(ITape& result, AdvancedTape& left, AdvancedTape& right)
{
    ITape& lhs{ *left.first };
    ITape& rhs{ *right.first };

    size_t left_tape_size{ left.second.size };
    size_t right_tape_size{ right.second.size };

    int32_t left_value{ get(lhs) };
    int32_t right_value{ get(rhs) };

    while (left_tape_size && right_tape_size)
    {
        if (left_value <= right_value)
        {
            set(result, left_value);
            if (--left_tape_size)
            {
                StepForward(lhs);
                left_value = get(lhs);
            }
        }
        else
        {
            set(result, right_value);
            if (--right_tape_size)
            {
                StepForward(rhs);
                right_value = get(rhs);
            }
        }
        StepForward(result);
    }

    if (left_tape_size)
    {
        set(result, left_value);
        while (--left_tape_size)
        {
            StepForward(result);
            StepForward(lhs);
            set(result, get(lhs));
        }
    }
    else
    {
        set(result, right_value);
        while (--right_tape_size)
        {
            StepForward(result);
            StepForward(rhs);
            set(result, get(rhs));
        }
    }
}