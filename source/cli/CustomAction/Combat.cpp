#include "Combat.h"

void combat_move(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView custom_action_param,
                 int32_t target_x, int32_t target_y, int32_t target_w, int32_t target_h)
{
    RandomPointGenerator generator;
    int32_t x1, y1, x2, y2;
    generator.generate_point(127, 550, 50, 50, x1, y1);
    generator.generate_point(target_x, target_y, target_w, target_h, x2, y2);

    MaaSyncContextTouchDown(sync_context, 0, x1, y1, 0);
    MaaSyncContextTouchMove(sync_context, 0, x2, y2, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    MaaSyncContextTouchUp(sync_context, 0);
}

MaaBool combat_move_forward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                            MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                            [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    combat_move(sync_context, custom_action_param, 149, 466, 6, 7);
    return true;
}

MaaBool combat_move_backward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                             MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                             [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    combat_move(sync_context, custom_action_param, 149, 684, 6, 7);
    return true;
}

MaaBool combat_move_left(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                         MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                         [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    combat_move(sync_context, custom_action_param, 36, 572, 7, 6);
    return true;
}

MaaBool combat_move_right(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                          MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                          [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    combat_move(sync_context, custom_action_param, 261, 572, 7, 6);
    return true;
}

MaaBool combat_move_left_forward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                                 MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                                 [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    combat_move(sync_context, custom_action_param, 70, 493, 5, 5);
    return true;
}

MaaBool combat_move_right_forward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                                  MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                                  [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    combat_move(sync_context, custom_action_param, 229, 493, 5, 5);
    return true;
}

MaaBool combat_move_left_backward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                                  MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                                  [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    combat_move(sync_context, custom_action_param, 70, 652, 5, 5);
    return true;
}

MaaBool combat_move_right_backward(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                                   MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                                   [[maybe_unused]] MaaStringView cur_rec_detail,
                                   [[maybe_unused]] MaaTransparentArg arg)
{
    combat_move(sync_context, custom_action_param, 229, 652, 5, 5);
    return true;
}

MaaBool combat_basic_ATK(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                         [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                         [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(1146, 606, 82, 64, x, y);

    MaaSyncContextClick(sync_context, x, y);
    return true;
}

MaaBool combat_basic_ATK_charge(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                                [[maybe_unused]] MaaStringView custom_action_param,
                                [[maybe_unused]] MaaRectHandle cur_box, [[maybe_unused]] MaaStringView cur_rec_detail,
                                [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(1146, 606, 82, 64, x, y);

    MaaSyncContextTouchDown(sync_context, 0, x, y, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    MaaSyncContextTouchUp(sync_context, 0);
    return true;
}

MaaBool combat_evade(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                     [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                     [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(971, 608, 79, 62, x, y);

    MaaSyncContextClick(sync_context, x, y);
    return true;
}

MaaBool combat_ultimate(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                        [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                        [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(1145, 424, 82, 74, x, y);

    MaaSyncContextClick(sync_context, x, y);
    return true;
}

MaaBool combat_ultimate_charge(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                               [[maybe_unused]] MaaStringView custom_action_param,
                               [[maybe_unused]] MaaRectHandle cur_box, [[maybe_unused]] MaaStringView cur_rec_detail,
                               [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(1145, 424, 82, 74, x, y);

    MaaSyncContextTouchDown(sync_context, 0, x, y, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    MaaSyncContextTouchUp(sync_context, 0);
    return true;
}

MaaBool combat_weapon_skill(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                            [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                            [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(977, 444, 69, 64, x, y);

    MaaSyncContextClick(sync_context, x, y);
    return true;
}

MaaBool combat_ELF_skill(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                         [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                         [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(857, 515, 63, 53, x, y);

    MaaSyncContextClick(sync_context, x, y);
    return true;
}

MaaBool combat_extra_skill(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                           [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                           [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(980, 294, 64, 49, x, y);

    MaaSyncContextClick(sync_context, x, y);
    return true;
}

MaaBool combat_QTE1(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                    [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                    [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(1157, 113, 73, 51, x, y);

    MaaSyncContextClick(sync_context, x, y);
    return true;
}

MaaBool combat_QTE2(MaaSyncContextHandle sync_context, [[maybe_unused]] MaaStringView task_name,
                    [[maybe_unused]] MaaStringView custom_action_param, [[maybe_unused]] MaaRectHandle cur_box,
                    [[maybe_unused]] MaaStringView cur_rec_detail, [[maybe_unused]] MaaTransparentArg arg)
{
    RandomPointGenerator generator;
    int32_t x, y;
    generator.generate_point(1160, 261, 70, 56, x, y);

    MaaSyncContextClick(sync_context, x, y);
    return true;
}