#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#define CONFIG_LOGGING

#ifdef CONFIG_LOGGING
#include <easylogging++.h>
#else
#define LOG(x)
#endif

namespace coco
{

using json = nlohmann::json;

class Config
{
  public:
    Config(const Config&) = delete;
    Config&        operator=(const Config&) = delete;
    static Config& getInstance()
    {
        static Config instance;
        return instance;
    }

    void load(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (file.is_open())
        {
            LOG(INFO) << "Using config file: " << filePath;
            file >> configData;
        }
        else
        {
            LOG(WARNING) << "Could not open config file: " << filePath;
        }
    }
    template <typename T> std::optional<T> getValue(const std::string& key) const
    {
        if (configData.contains(key))
        {
            return configData.at(key).get<T>();
        }
        else
        {
            LOG(WARNING) << "Could not find key: " << key;
            return std::nullopt;
        }
    }
    template <typename T> T getValue(const std::string& key, const T defaultValue) const
    {
        if (configData.contains(key))
        {
            return configData.at(key).get<T>();
        }
        else
        {
            LOG(WARNING) << "Could not find key: " << key << ". Using default value: " << defaultValue;
            return defaultValue;
        }
    }

  private:
    Config() = default;

    json configData;
};

} // namespace coco
