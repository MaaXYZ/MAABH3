#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

#include "Config.h"

class ControlConfig : public Config
{
public:
    ControlConfig();
    virtual ~ControlConfig() = default;

public:
    inline static const std::filesystem::path local_dir = ".";
    inline static const std::filesystem::path config_dir = local_dir / "config" / "control";

public:
    bool parse(const json::value& config_json) override;
    json::value to_json() override;

public:
    void set_config_server(int server);
    void set_config_screencap(int screencap);
    void set_config_touch(int touch);

public:
    int get_config_server();
    int get_config_screencap();
    int get_config_touch();

private:
    void build_target_path() override;

private:
    int _config_server = 1;
    int _config_screencap = 3;
    int _config_touch = 3;
};