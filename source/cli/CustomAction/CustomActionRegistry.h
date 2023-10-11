#pragma once

#include "Combat.h"
#include "CustomActionRegistrar.h"

void custom_action_init(CustomActionRegistrar& registrar)
{
    registrar.add_action("Forward", combat_forward);
    registrar.add_action("Backward", combat_backward);
    registrar.add_action("Left", combat_left);
    registrar.add_action("Right", combat_right);
    registrar.add_action("LeftForward", combat_left_forward);
    registrar.add_action("RightForward", combat_right_forward);
    registrar.add_action("LeftBackward", combat_left_backward);
    registrar.add_action("RightBackward", combat_right_backward);
    registrar.add_action("BasicATK", combat_basic_ATK);
    registrar.add_action("BasicATKChargeBase", combat_basic_ATK_charge_base);
    registrar.add_action("BasicATKCharge", combat_basic_ATK_charge);
    registrar.add_action("Evade", combat_evade);
    registrar.add_action("Ultimate", combat_ultimate);
    registrar.add_action("UltimateChargeBase", combat_ultimate_charge_base);
    registrar.add_action("UltimateCharge", combat_ultimate_charge);
    registrar.add_action("WeaponSkill", combat_weapon_skill);
    registrar.add_action("ELFSkill", combat_ELF_skill);
    registrar.add_action("ExtraSkill", combat_extra_skill);
    registrar.add_action("QTE1", combat_QTE1);
    registrar.add_action("QTE2", combat_QTE2);
}

void register_custom_action(MaaInstanceHandle maa_handle)
{
    CustomActionRegistrar registerar;
    custom_action_init(registerar);
    registerar.register_actions(maa_handle);
}