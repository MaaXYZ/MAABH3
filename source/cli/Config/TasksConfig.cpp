#include "TasksConfig.h"

TasksConfig::TasksConfig()
{
    build_target_path();
}

bool TasksConfig::parse(const json::value& config_opt)
{
    auto arr = config_opt.as_array();
    for (auto& el : arr) {
        Task task;
        task.name = el.get("name", "");
        task.status = el.get("status", true);
        if (!task.status) {
            continue;
        }
        task.param = el.get("param", json::object());
        task.type = el.get("type", "");
        _config_tasklist.emplace_back(std::move(task));
    }

    return true;
}

bool TasksConfig::load()
{
    auto config_opt = json::open(_target_path);
    if (!config_opt) {
        std::cerr << "Failed to open config file: " << _target_path << std::endl;
        return false;
    }

    auto& config = *config_opt;
    if (!config.is_array()) {
        std::cerr << "Json is not an array: " << config << std::endl;
        return false;
    }

    if (!parse(config)) {
        std::cerr << "Failed to parse control: " << config << std::endl;
        return false;
    }

    return true;
}

json::value TasksConfig::to_json()
{
    json::value root = json::array();
    for (auto& task : _config_tasklist) {
        json::object obj = {
            { "name", task.name }, { "status", task.status }, { "param", task.param }, { "type", task.type }
        };
        root.emplace(obj);
    }

    return root;
}

void TasksConfig::set_config_tasklist(std::vector<Task> tasklist)
{
    _config_tasklist = tasklist;
}

std::vector<Task> TasksConfig::get_config_tasklist()
{
    return _config_tasklist;
}

void TasksConfig::build_target_path()
{
    _target_path = config_dir / _target;
}
