#include "CustomRecognizer.h"

CustomRecognizer::CustomRecognizer(
    const std::string& recognizer_name,
    MaaBool (*recognizer_analyze)(MaaSyncContextHandle sync_context, const MaaImageBufferHandle image,
                                  MaaStringView task_name, MaaStringView custom_recognition_param,
                                  MaaTransparentArg recognizer_arg, MaaRectHandle out_box,
                                  MaaStringBufferHandle detail_buff))
    : name_(recognizer_name)
{
    custom_recognizer_.analyze = recognizer_analyze;
}

std::string CustomRecognizer::get_name() const
{
    return name_;
}

MaaCustomRecognizerAPI CustomRecognizer::get_custom_recognizer() const
{
    return custom_recognizer_;
}