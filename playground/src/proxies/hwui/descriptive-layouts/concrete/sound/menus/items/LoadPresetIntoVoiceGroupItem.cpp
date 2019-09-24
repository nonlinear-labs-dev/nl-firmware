#include "LoadPresetIntoVoiceGroupItem.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>

LoadPresetIntoVoiceGroupItem::LoadPresetIntoVoiceGroupItem()
    : BasicItem("Import Preset")
{
}

void LoadPresetIntoVoiceGroupItem::doAction()
{
  Application::get().getHWUI()->setFocusAndMode(UIDetail::SoundSelectPresetForVoiceGroup);
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().bruteForce();
}
