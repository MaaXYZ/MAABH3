#pragma once

#include <iostream>

#include "ControlConfig.h"
#include "DeviceConfig.h"
#include "TasksConfig.h"

enum ConfigOption
{
    ConfigOption_Control = 0,
    ConfigOption_Device = 1,
    ConfigOption_Tasks = 2
};

class ConfigMgr
{
public:
    static ConfigMgr& get_instance();

public:
    bool init();
    bool set_config_target(ConfigOption option, std::string target);

public:
    ControlConfig& get_control_config();
    DeviceConfig& get_device_config();
    TasksConfig& get_tasks_config();

private:
    ConfigMgr() = default;
    ~ConfigMgr() = default;

private:
    ControlConfig _control;
    DeviceConfig _device;
    TasksConfig _tasks;
};