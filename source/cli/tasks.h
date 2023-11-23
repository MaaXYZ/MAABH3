#pragma once

#include "meojson/json.hpp"
#include <MaaFramework/MaaDef.h>

#include "Config/TasksConfig.h"

bool default_tasks_init(TasksConfig& tasks);

bool select_tasks(std::vector<Task>& tasklist);

json::value dorm_param();

json::value universal_mirage_param();

json::value close_game_param();

bool app_package_and_activity(int client_type, std::string& package, std::string& activity);

std::string TaskStatus(MaaStatus status);