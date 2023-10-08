#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "MaaFramework/MaaAPI.h"
#include "MaaToolKit/MaaToolKitAPI.h"
#include "meojson/json.hpp"

#include "RegisterCustomAciton.h"

struct Task
{
    std::string name;
    std::string type;
    bool enabled = true;
    json::value param = json::object();
};

using TaskList = std::vector<Task>;

struct AfterTask
{
    std::string type;
    bool enabled = true;
    json::value param = json::object();
};

void print_help();

void print_version();

MaaSize scanning_devices();

json::value dorm_param();

json::value end_to_do_param();

bool proc_argv(int argc, char** argv, bool& debug, std::string& adb, std::string& adb_address, int& client_type,
               TaskList& tasks, AfterTask& after_task, MaaAdbControllerType& ctrl_type);

bool app_package_and_activity(int client_type, std::string& package, std::string& activity);

bool match_adb_address(const std::string& adb_address, MaaSize& index , const MaaSize& device_size);

void print_device_list(const MaaSize& device_size);

MaaSize get_device_index(const MaaSize& device_size);

void save_config(const std::string& adb, const std::string& adb_address, const int& client_type, const TaskList& tasks,
                 MaaAdbControllerType ctrl_type);

std::string read_adb_config(const std::filesystem::path& cur_dir);

std::string TaskStatus(MaaStatus status);

void mpause();