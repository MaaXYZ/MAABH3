#include "CombatRecognizer.h"

MaaBool combat_combating(MaaSyncContextHandle sync_context, const MaaImageBufferHandle image,
                         [[maybe_unused]] MaaStringView task_name,
                         [[maybe_unused]] MaaStringView custom_recognition_param,
                         [[maybe_unused]] MaaTransparentArg recognizer_arg, MaaRectHandle out_box,
                         MaaStringBufferHandle detail_buff)
{
    json::object combating { { "recognition", "TemplateMatch" },
                             { "template", "Combat/StopCombat.png" },
                             { "roi", json::array { 0, 0, 136, 140 } } };
    json::value diff_task { { "Combating", combating } };
    json::value task_param { { "diff_task", diff_task } };
    std::string task_param_str = task_param.to_string();

    MaaBool result =
        MaaSyncContextRunRecognition(sync_context, image, "Combating", task_param_str.c_str(), out_box, detail_buff);
    return result;
}