#pragma once

#include <MaaToolkit/MaaToolkitAPI.h>
#include <MaaFramework/MaaDef.h>

#include "Config/DeviceConfig.h"
#include "Cache/AdbConfigCache.h"

bool default_device_init(DeviceConfig& device);

MaaSize scanning_devices();

bool select_device(std::string& name, std::string& SN, std::string& adb);

bool select_device_number(const MaaSize& device_size, MaaSize& number);

void print_device_list(const MaaSize& device_size);

bool manually_enter_device_info(std::string& name, std::string& SN, std::string& adb);

bool manually_enter_device_name(std::string& name);

bool manually_enter_device_SN(std::string& SN);

bool manually_enter_device_adb(std::string& adb);