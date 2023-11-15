#include "CustomAction.h"

CustomAction::CustomAction(const std::string& action_name,
                           MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                                 MaaStringView custom_action_param, MaaRectHandle cur_box,
                                                 MaaStringView cur_rec_detail, MaaTransparentArg arg))
    : _name(action_name)
{
    _custom_action.run = action_run;
}

CustomAction::CustomAction(const std::string& action_name,
                           MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                                 MaaStringView custom_action_param, MaaRectHandle cur_box,
                                                 MaaStringView cur_rec_detail, MaaTransparentArg arg),
                           void (*action_stop)(MaaTransparentArg arg))
    : _name(action_name)
{
    _custom_action.run = action_run;
    _custom_action.stop = action_stop;
}

std::string CustomAction::get_name() const
{
    return _name;
}

MaaCustomActionAPI CustomAction::get_custom_action() const
{
    return _custom_action;
}