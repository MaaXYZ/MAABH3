#include "DeviceConfig.h"

DeviceConfig::DeviceConfig()
{
    build_target_path();
}
bool DeviceConfig::parse(const json::value& config_opt)
{
    _config_adb = config_opt.get("adb", _config_adb);
    _config_device_name = config_opt.get("device_name", _config_device_name);
    _config_device_SN = config_opt.get("device_SN", _config_device_SN);

    return true;
}
bool DeviceConfig::load()
{
    auto config_opt = json::open(_target_path);
    if (!config_opt) {
        std::cerr << "Failed to open device config file: " << _target_path << std::endl;
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

json::value DeviceConfig::to_json()
{
    json::value root = { { "device_name", _config_device_name },
                         { "device_SN", _config_device_SN },
                         { "adb", _config_adb } };

    return root;
}

bool DeviceConfig::save(const json::value& root)
{
    std::filesystem::create_directories(config_dir);
    std::ofstream ofs(_target_path, std::ios::out);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open device config file: " << _target_path << std::endl;
        return false;
    }
    ofs << root;
    ofs.close();
    return true;
}

void DeviceConfig::set_config_device_name(std::string name)
{
    _config_device_name = name;
}

void DeviceConfig::set_config_device_SN(std::string SN)
{
    _config_device_SN = SN;
}

void DeviceConfig::set_config_adb(std::string adb)
{
    _config_adb = adb;
}

std::string DeviceConfig::get_config_device_name()
{
    return _config_device_name;
}

std::string DeviceConfig::get_config_device_SN()
{
    return _config_device_SN;
}

std::string DeviceConfig::get_config_adb()
{
    return _config_adb;
}

void DeviceConfig::build_target_path()
{
    _target_path = config_dir / _target;
}
