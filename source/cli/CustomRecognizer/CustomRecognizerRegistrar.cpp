#include "CustomRecognizerRegistrar.h"

void CustomRecognizerRegistrar::add_recognizer(
    const std::string& name,
    MaaBool (*recognizer_analyze)(MaaSyncContextHandle sync_context, const MaaImageBufferHandle image,
                                  MaaStringView task_name, MaaStringView custom_recognition_param,
                                  MaaTransparentArg recognizer_arg, MaaRectHandle out_box,
                                  MaaStringBufferHandle detail_buff))
{
    recognizers_.emplace_back(name, recognizer_analyze);
}

void CustomRecognizerRegistrar::register_recognizers(MaaInstanceHandle maa_handle)
{
    for (const auto& recognizer : recognizers_) {
        std::shared_ptr<MaaCustomRecognizerAPI> custom_recognizer =
            std::make_shared<MaaCustomRecognizerAPI>(recognizer.get_custom_recognizer());
        custom_recognizers_.push_back(custom_recognizer);
        MaaRegisterCustomRecognizer(maa_handle, recognizer.get_name().c_str(), custom_recognizer.get(), nullptr);
    }
}