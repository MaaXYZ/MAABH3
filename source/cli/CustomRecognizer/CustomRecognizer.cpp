#include "CustomRecognizer.h"

CustomRecognizer::CustomRecognizer(
    const std::string& recognizer_name,
    MaaBool (*recognizer_analyze)(MaaSyncContextHandle sync_context, const MaaImageBufferHandle image,
                                  MaaStringView task_name, MaaStringView custom_recognition_param,
                                  MaaTransparentArg recognizer_arg, MaaRectHandle out_box,
                                  MaaStringBufferHandle detail_buff))
    : _name(recognizer_name)
{
    _custom_recognizer.analyze = recognizer_analyze;
}

std::string CustomRecognizer::get_name() const
{
    return _name;
}

MaaCustomRecognizerAPI CustomRecognizer::get_custom_recognizer() const
{
    return _custom_recognizer;
}