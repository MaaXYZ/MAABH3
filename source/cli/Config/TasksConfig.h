#pragma once

#include <vector>

#include "Config.h"

struct Task
{
    std::string name;
    bool status = true;
    std::string type;
    json::value param = json::object();
};

class TasksConfig : public Config
{
public:
    TasksConfig();
    virtual ~TasksConfig() = default;

public:
    inline static const std::filesystem::path local_dir = ".";
    inline static const std::filesystem::path config_dir = local_dir / "config" / "tasks"; 

public:
    bool parse(const json::value& config_opt) override;
    bool load() override;
    json::value to_json() override;

public:
    void set_config_tasklist(std::vector<Task> tasklist);

public:
    std::vector<Task> get_config_tasklist();

private:
    void build_target_path() override;

private:
    std::vector<Task> _config_tasklist;
};