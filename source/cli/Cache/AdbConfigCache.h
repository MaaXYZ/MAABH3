#pragma once

#include "Cache.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_map>

class AdbConfigCache : public Cache
{
public:
    virtual ~AdbConfigCache() = default;

public:
    inline static const std::filesystem::path adb_config_cache_dir = cache_dir / "adb_config";

public:
    bool has(const std::string& device_name, const std::string& device_SN);

public:
    bool set_adb_config(const std::string& device_name, const std::string& device_SN, const std::string& adb_config);
    std::string get_adb_config(const std::string& device_name, const std::string& device_SN);
    std::string get_default_adb_config();

private:
    std::string generate_cache_key(const std::string& device_name, const std::string& device_SN);
    bool save(const std::string& cache_key, const std::string& adb_config);
    bool load(const std::string& cache_key, std::string& adb_config);

private:
    std::unordered_map<std::string, std::string> cache_map;
};