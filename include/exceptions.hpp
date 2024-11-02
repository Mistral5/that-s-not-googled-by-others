#ifndef TAPESORT_INCLUDE_EXCEPTIONS_HPP_
#define TAPESORT_INCLUDE_EXCEPTIONS_HPP_

#include <exception>

class AssociatedFileException : public std::exception {
 public:
  explicit AssociatedFileException(const std::string& file_name)
      : message_{"File stream has no associated file with name: \"" +
                 file_name + "\""} {};

  const char* what() const throw() override { return message_.c_str(); }

 private:
  std::string message_;
};

class TempDirNotExistException : public std::exception {
 public:
  explicit TempDirNotExistException(const std::string& dir_name)
      : message_{"Temporary files directory \"" + dir_name +
                 "\" directory does not exist or is unavailable"} {};

  const char* what() const throw() override { return message_.c_str(); }

 private:
  std::string message_;
};

class MaxTempTapeSizeException : public std::exception {
 public:
  explicit MaxTempTapeSizeException()
      : message_{
            "The allocated memory must be enough for more than one "
            "(int32) element (check config)"} {};

  const char* what() const throw() override { return message_.c_str(); }

 private:
  std::string message_;
};

#endif  // TAPESORT_INCLUDE_EXCEPTIONS_HPP_