#pragma once

#include <chrono>
#include <string>
#include <thread>

#include "MaaFramework/Task/MaaSyncContext.h"

#include "RandomPointGenerator.h"

void combat_move(MaaSyncContextHandle sync_context, MaaStringView custom_action_param, int32_t target_x,
                 int32_t target_y, int32_t target_w, int32_t target_h);

MaaBool combat_move_forward(MaaSyncContextHandle sync_context, MaaStringView task_name,
                            MaaStringView custom_action_param, MaaRectHandle cur_box, MaaStringView cur_rec_detail,
                            MaaTransparentArg arg);
MaaBool combat_move_backward(MaaSyncContextHandle sync_context, MaaStringView task_name,
                             MaaStringView custom_action_param, MaaRectHandle cur_box, MaaStringView cur_rec_detail,
                             MaaTransparentArg arg);
MaaBool combat_move_left(MaaSyncContextHandle sync_context, MaaStringView task_name, MaaStringView custom_action_param,
                         MaaRectHandle cur_box, MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_move_right(MaaSyncContextHandle sync_context, MaaStringView task_name, MaaStringView custom_action_param,
                          MaaRectHandle cur_box, MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_move_left_forward(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                 MaaStringView custom_action_param, MaaRectHandle cur_box, MaaStringView cur_rec_detail,
                                 MaaTransparentArg arg);
MaaBool combat_move_right_forward(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                  MaaStringView custom_action_param, MaaRectHandle cur_box,
                                  MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_move_left_backward(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                  MaaStringView custom_action_param, MaaRectHandle cur_box,
                                  MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_move_right_backward(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                   MaaStringView custom_action_param, MaaRectHandle cur_box,
                                   MaaStringView cur_rec_detail, MaaTransparentArg arg);

MaaBool combat_basic_ATK(MaaSyncContextHandle sync_context, MaaStringView task_name, MaaStringView custom_action_param,
                         MaaRectHandle cur_box, MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_basic_ATK_charge(MaaSyncContextHandle sync_context, MaaStringView task_name,
                                MaaStringView custom_action_param, MaaRectHandle cur_box, MaaStringView cur_rec_detail,
                                MaaTransparentArg arg);
MaaBool combat_evade(MaaSyncContextHandle sync_context, MaaStringView task_name, MaaStringView custom_action_param,
                     MaaRectHandle cur_box, MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_ultimate(MaaSyncContextHandle sync_context, MaaStringView task_name, MaaStringView custom_action_param,
                        MaaRectHandle cur_box, MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_ultimate_charge(MaaSyncContextHandle sync_context, MaaStringView task_name,
                               MaaStringView custom_action_param, MaaRectHandle cur_box, MaaStringView cur_rec_detail,
                               MaaTransparentArg arg);
MaaBool combat_weapon_skill(MaaSyncContextHandle sync_context, MaaStringView task_name,
                            MaaStringView custom_action_param, MaaRectHandle cur_box, MaaStringView cur_rec_detail,
                            MaaTransparentArg arg);
MaaBool combat_ELF_skill(MaaSyncContextHandle sync_context, MaaStringView task_name, MaaStringView custom_action_param,
                         MaaRectHandle cur_box, MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_extra_skill(MaaSyncContextHandle sync_context, MaaStringView task_name,
                           MaaStringView custom_action_param, MaaRectHandle cur_box, MaaStringView cur_rec_detail,
                           MaaTransparentArg arg);
MaaBool combat_QTE1(MaaSyncContextHandle sync_context, MaaStringView task_name, MaaStringView custom_action_param,
                    MaaRectHandle cur_box, MaaStringView cur_rec_detail, MaaTransparentArg arg);
MaaBool combat_QTE2(MaaSyncContextHandle sync_context, MaaStringView task_name, MaaStringView custom_action_param,
                    MaaRectHandle cur_box, MaaStringView cur_rec_detail, MaaTransparentArg arg);