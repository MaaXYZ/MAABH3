#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CustomAction.h"
#include "MaaFramework/Instance/MaaInstance.h"
#include "MaaFramework/Task/MaaCustomAction.h"

class CustomActionRegistrar
{
public:
    CustomActionRegistrar() = default;
    ~CustomActionRegistrar() = default;

public:
    void add_action(const std::string& name,
                    MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                          MaaStringView custom_action_param, MaaRectHandle cur_box,
                                          MaaStringView cur_rec_detail, MaaTransparentArg arg));
    void add_action(const std::string& name,
                    MaaBool (*action_run)(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                          MaaStringView custom_action_param, MaaRectHandle cur_box,
                                          MaaStringView cur_rec_detail, MaaTransparentArg arg),
                    void (*action_stop)(MaaTransparentArg arg));
    void register_actions(MaaInstanceHandle maa_handle);

private:
    std::vector<CustomAction> actions_;
    std::vector<std::shared_ptr<MaaCustomActionAPI>> custom_actions_;
};