#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CustomRecognizer.h"
#include "MaaFramework/Instance/MaaInstance.h"
#include "MaaFramework/Task/MaaCustomRecognizer.h"

class CustomRecognizerRegistrar
{
public:
    CustomRecognizerRegistrar() = default;
    ~CustomRecognizerRegistrar() = default;

public:
    void add_recognizer(const std::string& name,
                        MaaBool (*recognizer_analyze)(MaaSyncContextHandle sync_context,
                                                      const MaaImageBufferHandle image, MaaStringView task_name,
                                                      MaaStringView custom_recognition_param,
                                                      MaaTransparentArg recognizer_arg, MaaRectHandle out_box,
                                                      MaaStringBufferHandle detail_buff));
    void register_recognizers(MaaInstanceHandle maa_handle);

private:
    std::vector<CustomRecognizer> _recognizers;
    std::vector<std::shared_ptr<MaaCustomRecognizerAPI>> _custom_recognizers;
};