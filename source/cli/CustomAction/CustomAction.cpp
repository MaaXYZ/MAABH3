#include "CustomAction.h"

CustomAction::CustomAction(const std::string& action_name,
                           MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                                 MaaStringView custom_action_param, MaaRectHandle cur_box,
                                                 MaaStringView cur_rec_detail, MaaTransparentArg arg))
    : name_(action_name)
{
    custom_action_.run = action_run;
}

CustomAction::CustomAction(const std::string& action_name,
                           MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                                 MaaStringView custom_action_param, MaaRectHandle cur_box,
                                                 MaaStringView cur_rec_detail, MaaTransparentArg arg),
                           void (*action_stop)(MaaTransparentArg arg))
    : name_(action_name)
{
    custom_action_.run = action_run;
    custom_action_.stop = action_stop;
}

std::string CustomAction::get_name() const
{
    return name_;
}

MaaCustomActionAPI CustomAction::get_custom_action() const
{
    return custom_action_;
}