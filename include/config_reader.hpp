#ifndef TAPESORT_SRC_CONFIG_READER_HPP
#define TAPESORT_SRC_CONFIG_READER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "exceptions.hpp"

class ConfigReader
{
public:
    ConfigReader(const std::string&);
    ~ConfigReader();

    template <typename T>
    const T get(const std::string& key) const
    {
        return config_json_.at(key).get<T>();
    };

    template <typename T>
    const T get(const std::string& target, const std::string& key) const
    {
        return config_json_.at(target).at(key).get<T>();
    };

private:
    nlohmann::json config_json_;
};

#endif // TAPESORT_SRC_CONFIG_READER_HPP