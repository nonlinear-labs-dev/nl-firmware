#pragma once

#include <presets/Preset.h>

struct HiddenMultiBank : public UpdateDocumentContributor {
    HiddenMultiBank(UpdateDocumentContributor *parent);

    std::vector<std::shared_ptr<Preset>> m_presets;
};
