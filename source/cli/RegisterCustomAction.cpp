#include "RegisterCustomAciton.h"

MaaCustomActionAPI action_combat_forward;
MaaCustomActionAPI action_combat_backward;
MaaCustomActionAPI action_combat_left;
MaaCustomActionAPI action_combat_right;
MaaCustomActionAPI action_combat_left_forward;
MaaCustomActionAPI action_combat_right_forward;
MaaCustomActionAPI action_combat_left_backward;
MaaCustomActionAPI action_combat_right_backward;
MaaCustomActionAPI action_combat_basic_ATK;
MaaCustomActionAPI action_combat_basic_ATK_charge_base;
MaaCustomActionAPI action_combat_basic_ATK_charge;
MaaCustomActionAPI action_combat_evade;
MaaCustomActionAPI action_combat_ultimate;
MaaCustomActionAPI action_combat_ultimate_charge_base;
MaaCustomActionAPI action_combat_ultimate_charge;
MaaCustomActionAPI action_combat_weapon_skill;
MaaCustomActionAPI action_combat_ELF_skill;
MaaCustomActionAPI action_combat_extra_skill;
MaaCustomActionAPI action_combat_QTE1;
MaaCustomActionAPI action_combat_QTE2;

void custom_action_init()
{
    action_combat_forward.run = combat_forward;
    action_combat_backward.run = combat_backward;
    action_combat_left.run = combat_left;
    action_combat_right.run = combat_right;
    action_combat_left_forward.run = combat_left_forward;
    action_combat_right_forward.run = combat_right_forward;
    action_combat_left_backward.run = combat_left_backward;
    action_combat_right_backward.run = combat_right_backward;
    action_combat_basic_ATK.run = combat_basic_ATK;
    action_combat_basic_ATK_charge_base.run = combat_basic_ATK_charge_base;
    action_combat_basic_ATK_charge.run = combat_basic_ATK_charge;
    action_combat_evade.run = combat_evade;
    action_combat_ultimate.run = combat_ultimate;
    action_combat_ultimate_charge_base.run = combat_ultimate_charge_base;
    action_combat_ultimate_charge.run = combat_ultimate_charge;
    action_combat_weapon_skill.run = combat_weapon_skill;
    action_combat_ELF_skill.run = combat_ELF_skill;
    action_combat_extra_skill.run = combat_extra_skill;
    action_combat_QTE1.run = combat_QTE1;
    action_combat_QTE2.run = combat_QTE2;
}

void register_custom_action(MaaInstanceHandle maa_handle)
{
    custom_action_init();

    MaaRegisterCustomAction(maa_handle, "Forward", &action_combat_forward, nullptr);
    MaaRegisterCustomAction(maa_handle, "Backward", &action_combat_backward, nullptr);
    MaaRegisterCustomAction(maa_handle, "Left", &action_combat_left, nullptr);
    MaaRegisterCustomAction(maa_handle, "Right", &action_combat_right, nullptr);
    MaaRegisterCustomAction(maa_handle, "LeftForward", &action_combat_left_forward, nullptr);
    MaaRegisterCustomAction(maa_handle, "RightForward", &action_combat_right_forward, nullptr);
    MaaRegisterCustomAction(maa_handle, "LeftBackward", &action_combat_left_backward, nullptr);
    MaaRegisterCustomAction(maa_handle, "RightBackward", &action_combat_right_backward, nullptr);
    MaaRegisterCustomAction(maa_handle, "BasicATK", &action_combat_basic_ATK, nullptr);
    MaaRegisterCustomAction(maa_handle, "BasicATKChargeBase", &action_combat_basic_ATK_charge_base, nullptr);
    MaaRegisterCustomAction(maa_handle, "BasicATKCharge", &action_combat_basic_ATK_charge, nullptr);
    MaaRegisterCustomAction(maa_handle, "Evade", &action_combat_evade, nullptr);
    MaaRegisterCustomAction(maa_handle, "Ultimate", &action_combat_ultimate, nullptr);
    MaaRegisterCustomAction(maa_handle, "UltimateChargeBase", &action_combat_ultimate_charge_base, nullptr);
    MaaRegisterCustomAction(maa_handle, "UltimateCharge", &action_combat_ultimate_charge, nullptr);
    MaaRegisterCustomAction(maa_handle, "WeaponSkill", &action_combat_weapon_skill, nullptr);
    MaaRegisterCustomAction(maa_handle, "ELFSkill", &action_combat_ELF_skill, nullptr);
    MaaRegisterCustomAction(maa_handle, "ExtraSkill", &action_combat_extra_skill, nullptr);
    MaaRegisterCustomAction(maa_handle, "QTE1", &action_combat_QTE1, nullptr);
    MaaRegisterCustomAction(maa_handle, "QTE2", &action_combat_QTE2, nullptr);
}