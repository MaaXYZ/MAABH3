#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <MaaFramework/MaaAPI.h>
#include <MaaToolKit/MaaToolKitAPI.h>

#include "Cache/AdbConfigCache.h"
#include "Config/ConfigMgr.h"

#include "CustomAction/CustomActionRegistry.h"
#include "CustomRecognizer/CustomRecognizerRegistry.h"

#include "control.h"
#include "device.h"
#include "tasks.h"

bool parse_param(int argc, char** argv, std::unordered_map<std::string, std::string>& options,
                 std::unordered_map<std::string, bool>& flags);

void print_help();

void print_version();

void mpause();