#pragma once

#include <string>

#include "MaaFramework/Task/MaaCustomAction.h"

class CustomAction
{
public:
    CustomAction(const std::string& action_name,
                 MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                       MaaStringView custom_action_param, MaaRectHandle cur_box,
                                       MaaStringView cur_rec_detail, MaaTransparentArg arg));
    CustomAction(const std::string& action_name,
                 MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                       MaaStringView custom_action_param, MaaRectHandle cur_box,
                                       MaaStringView cur_rec_detail, MaaTransparentArg arg),
                 void (*action_stop)(MaaTransparentArg arg));
    ~CustomAction() = default;

public:
    std::string get_name() const;
    MaaCustomActionAPI get_custom_action() const;

private:
    std::string name_;
    MaaCustomActionAPI custom_action_;
};