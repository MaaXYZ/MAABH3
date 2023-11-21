#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "MaaFramework/MaaAPI.h"
#include "MaaToolKit/MaaToolKitAPI.h"
#include "meojson/json.hpp"

#include "Cache/AdbConfigCache.h"
#include "Config/ConfigMgr.h"

#include "CustomAction/CustomActionRegistry.h"
#include "CustomRecognizer/CustomRecognizerRegistry.h"

void print_help();

void print_version();

MaaSize scanning_devices();

bool default_control_init(ControlConfig& control);

bool select_server(int& server);

bool default_device_init(DeviceConfig& device);

bool select_device(std::string& name, std::string& SN, std::string& adb);

bool select_device_number(const MaaSize& device_size, MaaSize& number);

void print_device_list(const MaaSize& device_size);

bool manually_enter_device_info(std::string& name, std::string& SN, std::string& adb);

bool manually_enter_device_name(std::string& name);

bool manually_enter_device_SN(std::string& SN);

bool manually_enter_device_adb(std::string& adb);

bool default_tasks_init(TasksConfig& tasks);

bool select_tasks(std::vector<Task>& tasklist);

json::value dorm_param();

json::value universal_mirage_param();

json::value close_game_param();

bool app_package_and_activity(int client_type, std::string& package, std::string& activity);

bool match_adb_address(const std::string& adb_address, MaaSize& index, const MaaSize& device_size);

std::string TaskStatus(MaaStatus status);

void mpause();