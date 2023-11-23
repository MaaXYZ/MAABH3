#pragma once

#include "Config/ControlConfig.h"

bool default_control_init(ControlConfig& control);

bool select_server(int& server);

bool app_package_and_activity(int client_type, std::string& package, std::string& activity);