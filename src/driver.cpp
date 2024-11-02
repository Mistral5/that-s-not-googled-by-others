#include "driver.hpp"

using std::chrono::milliseconds;
using std::this_thread::sleep_for;

tape_sort::Driver::Driver(const DriverParams& driver_params,
                          ITapeFactory& factory)
    : kDriverParams(driver_params), factory_(factory){};

tape_sort::Driver::~Driver() = default;

int32_t tape_sort::Driver::get(ITape& tape) {
  ++drive_perf_stats_.reading_count_;
  sleep_for(milliseconds(kDriverParams.item_read_delay));
  return tape.get();
}

void tape_sort::Driver::set(ITape& tape, int32_t value) {
  ++drive_perf_stats_.writing_count_;
  sleep_for(milliseconds(kDriverParams.item_write_delay));
  return tape.set(value);
}

void tape_sort::Driver::Rewind(ITape& tape, TapePosition pos) {
  ++drive_perf_stats_.rewind_count_;
  sleep_for(milliseconds(kDriverParams.tape_rewind_delay));
  return tape.Rewind(pos);
}

void tape_sort::Driver::StepForward(ITape& tape) {
  ++drive_perf_stats_.step_shift_count_;
  sleep_for(milliseconds(kDriverParams.step_shift_delay));
  return tape.StepForward();
}

std::string tape_sort::Driver::TapeTitleCreate(TapeParams& params) {
  return std::string(std::to_string(params.begin) + '-' +
                     std::to_string(params.end) + '_' +
                     std::to_string(params.size));
}

void tape_sort::Driver::ToTape(ITape& output, std::vector<int32_t>& input) {
  for (auto el{input.begin()}; el != std::prev(input.end()); ++el) {
    set(output, *el);
    StepForward(output);
  }
  set(output, *std::prev(input.end()));
}

void tape_sort::Driver::Sort(ITape& output, ITape& input) {
  AdvancedTapeQueue tape_storage{CreateTapePriorityQueue(input)};

  if (tape_storage.empty()) return;

  while (tape_storage.size() > kDriverParams.elem_num) MergeSort(tape_storage);

  Merge(output, tape_storage);
}

tape_sort::Driver::AdvancedTapeQueue tape_sort::Driver::CreateTapePriorityQueue(
    ITape& input) {
  size_t input_size{0};
  AdvancedTapeQueue tape_storage;

  while (input.good()) {
    size_t curr_tape_start{input_size};
    std::vector<int32_t> storage;
    size_t remaining_size{kDriverParams.elem_num};
    storage.reserve(remaining_size);

    while (remaining_size && input.good()) {
      storage.push_back(get(input));
      StepForward(input);
      --remaining_size;
    }

    std::sort(storage.begin(), storage.end());

    input_size += kDriverParams.elem_num - remaining_size;
    TapeParams curr_tape_params{curr_tape_start, input_size - 1,
                                input_size - curr_tape_start};

    std::string curr_tape_name{std::move(TapeTitleCreate(curr_tape_params))};

    std::shared_ptr<ITape> curr_tape{factory_.Create(curr_tape_name)};

    ToTape(*curr_tape, storage);
    Rewind(*curr_tape, TapePosition::kStart);

    tape_storage.push_back({curr_tape, curr_tape_params});
  }

  return tape_storage;
}

void tape_sort::Driver::MergeSort(AdvancedTapeQueue& queue) {
  AdvancedTapeQueue temp_tapes;

  temp_tapes.insert(
      temp_tapes.end(), std::make_move_iterator(queue.begin()),
      std::make_move_iterator(queue.begin() + kDriverParams.elem_num));

  queue.erase(queue.begin(), queue.begin() + kDriverParams.elem_num);

  size_t begin{temp_tapes[0].second.begin};
  size_t end{temp_tapes[0].second.end};
  size_t size{0};

  for (AdvancedTape i : temp_tapes) {
    begin = std::min(begin, i.second.begin);
    end = std::max(end, i.second.end);
    size += i.second.size;
  }

  TapeParams result_tape_params{begin, end, size};
  std::string curr_tape_name{std::move(TapeTitleCreate(result_tape_params))};

  std::shared_ptr<ITape> result_tape{factory_.Create(curr_tape_name)};

  Merge(*result_tape, temp_tapes);
  Rewind(*result_tape, TapePosition::kStart);

  queue.push_back({result_tape, result_tape_params});
}

void tape_sort::Driver::Merge(ITape& result, AdvancedTapeQueue& tapes) {
  std::vector<size_t> counters;

  using PairValueTape = std::pair<int32_t, size_t>;

  std::priority_queue<PairValueTape, std::vector<PairValueTape>,
                      std::greater<PairValueTape>>
      queue;

  size_t counter{0};
  for (AdvancedTape i : tapes) {
    counters.push_back(i.second.size - 1);
    queue.push(std::make_pair(get(*i.first), counter));
    ++counter;
  }

  while (queue.size()) {
    PairValueTape min{std::move(queue.top())};
    queue.pop();
    set(result, min.first);
    StepForward(result);
    if (counters[min.second]) {
      StepForward(*tapes[min.second].first);
      queue.push(std::make_pair(get(*tapes[min.second].first), min.second));
      --counters[min.second];
    }
  }
}