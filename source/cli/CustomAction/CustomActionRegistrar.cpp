#include "CustomActionRegistrar.h"

void CustomActionRegistrar::add_action(const std::string& name,
                                       MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                                             MaaStringView custom_action_param, MaaRectHandle cur_box,
                                                             MaaStringView cur_rec_detail, MaaTransparentArg arg))
{
    actions_.emplace_back(name, action_run);
}

void CustomActionRegistrar::add_action(const std::string& name,
                                       MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                                             MaaStringView custom_action_param, MaaRectHandle cur_box,
                                                             MaaStringView cur_rec_detail, MaaTransparentArg arg),
                                       void (*action_stop)(MaaTransparentArg arg))
{
    actions_.emplace_back(name, action_run, action_stop);
}

void CustomActionRegistrar::register_actions(MaaInstanceHandle maa_handle)
{
    for (const auto& action : actions_) {
        std::shared_ptr<MaaCustomActionAPI> custom_action =
            std::make_shared<MaaCustomActionAPI>(action.get_custom_action());
        custom_actions_.push_back(custom_action);
        MaaRegisterCustomAction(maa_handle, action.get_name().c_str(), custom_action.get(), nullptr);
    }
}