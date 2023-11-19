#pragma once

#include <filesystem>
#include <iostream>
#include <string>

#include "../meojson/json.hpp"

class Config
{
public:
    Config();
    virtual ~Config() = default;

public:
    virtual bool exists();
    virtual bool parse(const json::value& config_json) = 0;
    virtual bool load();
    virtual json::value to_json() = 0;
    virtual bool save(const json::value& root);
    bool dump();

public:
    virtual void set_target(const std::string target);

private:
    virtual void build_target_path() = 0;

protected:
    std::string _target = "default";
    std::filesystem::path _target_path = ".";
};