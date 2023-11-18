#pragma once

#include <string>

#include "../meojson/json.hpp"

class Config
{
public:
    virtual ~Config() = default;

public:
    virtual bool parse(const json::value& config_json) = 0;
    virtual bool load() = 0;
    virtual json::value to_json() = 0;
    virtual bool save(const json::value& root) = 0;
    bool dump();

public:
    virtual void set_target(const std::string target);

private:
    virtual void build_target_path() = 0;

protected:
    std::string _target = "default";
};