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

json::value DeviceConfig::to_json()
{
    json::value root = { { "device_name", _config_device_name },
                         { "device_SN", _config_device_SN },
                         { "adb", _config_adb } };

    return root;
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
