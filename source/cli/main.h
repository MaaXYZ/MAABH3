#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "MaaFramework/MaaAPI.h"
#include "MaaToolKit/MaaToolKitAPI.h"
#include "meojson/json.hpp"

struct Task
{
    std::string name;
    std::string type;
    bool enabled = true;
    json::value param = json::object();
};

using TaskList = std::vector<Task>;

void print_help();

bool proc_argv(int argc, char** argv, bool& debug, std::string& adb, std::string& adb_address, int& client_type,
               TaskList& tasks, MaaAdbControllerType& ctrl_type);

bool app_package_and_activity(int client_type, std::string& package, std::string& activity);

json::value homeland_param();

void save_config(const std::string& adb, const std::string& adb_address, const int& client_type, const TaskList& tasks,
                 MaaAdbControllerType ctrl_type);

std::string read_adb_config(const std::filesystem::path& cur_dir);

void mpause();