#include "Config.h"

bool Config::load()
{
    auto config_opt = json::open(_target_path);
    if (!config_opt) {
        std::cerr << "Failed to open config file: " << _target_path << std::endl;
        return false;
    }

    auto& config = *config_opt;
    if (!config.is_object()) {
        std::cerr << "Json is not an object: " << config << std::endl;
        return false;
    }

    if (!parse(config)) {
        std::cerr << "Failed to parse control: " << config << std::endl;
        return false;
    }

    return true;
}

bool Config::save(const json::value& root)
{
    std::filesystem::create_directories(_target_path.parent_path());
    std::ofstream ofs(_target_path, std::ios::out);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open config file: " << _target_path << std::endl;
        return false;
    }
    ofs << root;
    ofs.close();
    return true;
}

bool Config::dump()
{
    return save(to_json());
}

void Config::set_target(std::string target)
{
    _target = target;
    if (!_target.ends_with(".json")) {
        _target += ".json";
    }
    build_target_path();
}