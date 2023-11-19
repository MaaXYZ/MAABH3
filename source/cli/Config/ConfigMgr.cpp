#include "ConfigMgr.h"

ConfigMgr& ConfigMgr::get_instance()
{
    static ConfigMgr instance;
    return instance;
}

bool ConfigMgr::init()
{
    if (!_control.load()) {
        std::cerr << "Failed to load control config" << std::endl;
        return false;
    }
    if (!_device.load()) {
        std::cerr << "Failed to load device config" << std::endl;
        return false;
    }
    if (!_tasks.load()) {
        std::cerr << "Failed to load tasks config" << std::endl;
        return false;
    }
    return true;
}

bool ConfigMgr::set_config_target(ConfigOption option, std::string target)
{
    switch (option) {
    case ConfigOption_Control:
        _control.set_target(target);
        break;
    case ConfigOption_Device:
        _device.set_target(target);
        break;
    case ConfigOption_Tasks:
        _tasks.set_target(target);
        break;
    default:
        return false;
    }
    return true;
}

ControlConfig& ConfigMgr::get_control_config()
{
    return _control;
}

DeviceConfig& ConfigMgr::get_device_config()
{
    return _device;
}

TasksConfig& ConfigMgr::get_tasks_config()
{
    return _tasks;
}
