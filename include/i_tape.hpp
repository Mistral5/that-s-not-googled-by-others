#ifndef TAPESORT_INCLUDE_ITAPE_HPP_
#define TAPESORT_INCLUDE_ITAPE_HPP_

#include <cinttypes>
#include <fstream>

namespace tape_sort {
enum TapePosition { kStart = 0, kEnd = 2 };

class ITape {
 protected:
  ITape() = default;

 public:
  ITape(const ITape&) = delete;
  ITape& operator=(const ITape&) = delete;
  ITape(ITape&&) noexcept = default;
  ITape& operator=(ITape&&) noexcept = default;
  virtual ~ITape() = default;

  virtual int32_t get() = 0;
  virtual void set(int32_t) = 0;
  virtual void Rewind(TapePosition) = 0;
  virtual void StepForward() = 0;
  virtual bool good() = 0;
};

class ITapeFactory {
 public:
  virtual std::shared_ptr<ITape> Create(const std::string&) = 0;

  virtual ~ITapeFactory() {}
};
}  // namespace tape_sort

#endif  // TAPESORT_INCLUDE_ITAPE_HPP_