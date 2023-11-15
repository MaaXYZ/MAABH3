#pragma once

#include <string>

#include "MaaFramework/Task/MaaCustomRecognizer.h"

class CustomRecognizer
{
public:
    CustomRecognizer(const std::string& recognizer_name,
                     MaaBool (*recognizer_analyze)(MaaSyncContextHandle sync_context, const MaaImageBufferHandle image,
                                                   MaaStringView task_name, MaaStringView custom_recognition_param,
                                                   MaaTransparentArg recognizer_arg, MaaRectHandle out_box,
                                                   MaaStringBufferHandle detail_buff));
    ~CustomRecognizer() = default;

public:
    std::string get_name() const;
    MaaCustomRecognizerAPI get_custom_recognizer() const;

private:
    std::string _name;
    MaaCustomRecognizerAPI _custom_recognizer;
};