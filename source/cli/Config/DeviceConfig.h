#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

#include "Config.h"

class DeviceConfig : public Config
{
public:
    DeviceConfig();
    virtual ~DeviceConfig() = default;

public:
    inline static const std::filesystem::path local_dir = ".";
    inline static const std::filesystem::path config_dir = local_dir / "config" / "device";

public:
    bool parse(const json::value& config_opt) override;
    bool load() override;
    json::value to_json() override;
    bool save(const json::value& root) override;

public:
    void set_config_device_name(std::string name);
    void set_config_device_SN(std::string SN);
    void set_config_adb(std::string adb);

public:
    std::string get_config_device_name();
    std::string get_config_device_SN();
    std::string get_config_adb();

private:
    void build_target_path() override;

private:
    std::string _config_device_name;
    std::string _config_device_SN;
    std::string _config_adb;

private:
    std::filesystem::path _target_path;
};