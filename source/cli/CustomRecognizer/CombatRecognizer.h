#pragma once

#include <string>

#include "MaaFramework/Task/MaaSyncContext.h"
#include "MaaFramework/Utility/MaaBuffer.h"
#include "../meojson/json.hpp"

MaaBool combat_combating(MaaSyncContextHandle sync_context, const MaaImageBufferHandle image, MaaStringView task_name,
                  MaaStringView custom_recognition_param, MaaTransparentArg recognizer_arg, MaaRectHandle out_box,
                  MaaStringBufferHandle detail_buff);