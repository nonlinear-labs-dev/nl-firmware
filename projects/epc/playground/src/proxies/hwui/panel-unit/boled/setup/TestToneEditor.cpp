#include "TestToneEditor.h"
#include "proxies/hwui/controls/Label.h"
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>

TestToneEditor::TestToneEditor()
    : EnumSettingEditor<TestToneTypeSetting>()
{
}

TestToneEditor::~TestToneEditor()
{
  getSetting()->set(TestToneType::Off);
}
