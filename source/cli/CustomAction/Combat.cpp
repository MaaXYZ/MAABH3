#include "Combat.h"

MaaBool combat_forward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                       [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                       [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatForward", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_backward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                        [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                        [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatBackward", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_left(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                    [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                    [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatLeft", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_right(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                     [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                     [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatRight", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_left_forward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                            [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                            [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatLeftForward", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_right_forward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                             [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                             [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatRightForward", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_left_backward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                             [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                             [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatLeftBackward", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_right_backward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                              [[maybe_unused]] MaaStringView custom_action_param,
                              [[maybe_unused]] MaaRectHandle cur_box, [[maybe_unused]] MaaStringView cur_rec_detail,
                              [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatRightBackward", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_basic_ATK(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                         [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                         [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatBasicATK", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_basic_ATK_charge_base(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                                     [[maybe_unused]] MaaStringView custom_action_param,
                                     [[maybe_unused]] MaaRectHandle cur_box,
                                     [[maybe_unused]] MaaStringView cur_rec_detail,
                                     [[maybe_unused]] MaaTransparentArg arg)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int32_t> uni_x(1146, 1228);
    std::uniform_int_distribution<int32_t> uni_y(606, 670);
    int32_t x = uni_x(rng);
    int32_t y = uni_y(rng);

    MaaSyncContextTouchDown(sync_context, 0, x, y, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    MaaSyncContextTouchUp(sync_context, 0);
    return true;
}

MaaBool combat_basic_ATK_charge(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                                [[maybe_unused]] MaaStringView custom_action_param,
                                [[maybe_unused]] MaaRectHandle cur_box, [[maybe_unused]] MaaStringView cur_rec_detail,
                                [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatBasicATKCharge", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_evade(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                     [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                     [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatEvade", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_ultimate(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                        [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                        [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatUltimate", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_ultimate_charge_base(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                                    [[maybe_unused]] MaaStringView custom_action_param,
                                    [[maybe_unused]] MaaRectHandle cur_box,
                                    [[maybe_unused]] MaaStringView cur_rec_detail,
                                    [[maybe_unused]] MaaTransparentArg arg)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int32_t> uni_x(1145, 1227);
    std::uniform_int_distribution<int32_t> uni_y(424, 498);
    int32_t x = uni_x(rng);
    int32_t y = uni_y(rng);

    MaaSyncContextTouchDown(sync_context, 0, x, y, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    MaaSyncContextTouchUp(sync_context, 0);
    return true;
}

MaaBool combat_ultimate_charge(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                               [[maybe_unused]] MaaStringView custom_action_param,
                               [[maybe_unused]] MaaRectHandle cur_box, [[maybe_unused]] MaaStringView cur_rec_detail,
                               [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatUltimateCharge", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_weapon_skill(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                            [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                            [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatWeaponSkill", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_ELF_skill(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                         [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                         [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatELFSkill", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_extra_skill(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                           [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                           [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatExtraSkill", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_QTE1(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                    [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                    [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatQTE1", MaaTaskParam_Empty);
    return true;
}

MaaBool combat_QTE2(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                    [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                    [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    MaaSyncContextRunTask(sync_context, "CombatQTE2", MaaTaskParam_Empty);
    return true;
}