#include "ControlConfig.h"

ControlConfig::ControlConfig()
{
    build_target_path();
}

bool ControlConfig::parse(const json::value& config_opt)
{
    _config_server = config_opt.get("server", _config_server);
    _config_screencap = config_opt.get("screencap", _config_screencap);
    _config_touch = config_opt.get("touch", _config_touch);

    return true;
}

bool ControlConfig::load()
{
    auto config_opt = json::open(_target_path);
    if (!config_opt) {
        std::cerr << "Failed to open control config file: " << _target_path << std::endl;
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

json::value ControlConfig::to_json()
{
    json::value root;
    root = { { "server", _config_server }, { "screencap", _config_screencap }, { "touch", _config_touch } };
    return root;
}

bool ControlConfig::save(const json::value& root)
{
    std::filesystem::create_directories(config_dir);
    std::ofstream ofs(_target_path, std::ios::out);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open control config file: " << _target_path << std::endl;
        return false;
    }
    ofs << root;
    ofs.close();
    return true;
}

void ControlConfig::set_config_server(int server)
{
    _config_server = server;
}

void ControlConfig::set_config_screencap(int screencap)
{
    _config_screencap = screencap;
}

void ControlConfig::set_config_touch(int touch)
{
    _config_touch = touch;
}

int ControlConfig::get_config_server()
{
    return _config_server;
}

int ControlConfig::get_config_screencap()
{
    return _config_screencap;
}

int ControlConfig::get_config_touch()
{
    return _config_touch;
}

void ControlConfig::build_target_path()
{
    _target_path = config_dir / _target;
}
