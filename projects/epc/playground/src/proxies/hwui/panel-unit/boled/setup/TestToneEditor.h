#pragma once
#include "SetupEditor.h"
#include "proxies/hwui/controls/ControlWithChildren.h"
#include "SettingEditors.h"
#include "device-settings/TestToneTypeSetting.h"

class TestToneEditor : public EnumSettingEditor<TestToneTypeSetting> {
public:
    TestToneEditor();

    ~TestToneEditor() override;
};
