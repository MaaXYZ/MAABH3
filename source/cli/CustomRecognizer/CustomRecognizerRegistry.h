#pragma once

#include "CustomRecognizerRegistrar.h"

#include "CombatRecognizer.h"

void custom_recognizer_init(CustomRecognizerRegistrar& registrar)
{
    registrar.add_recognizer("Combating", combat_combating);
}

void register_custom_recognizer(MaaInstanceHandle maa_handle, CustomRecognizerRegistrar& registerar)
{
    custom_recognizer_init(registerar);
    registerar.register_recognizers(maa_handle);
}