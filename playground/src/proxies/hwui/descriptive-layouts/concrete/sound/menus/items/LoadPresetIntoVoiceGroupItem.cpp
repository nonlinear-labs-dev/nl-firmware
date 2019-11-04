#include "LoadPresetIntoVoiceGroupItem.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>

LoadPresetIntoVoiceGroupItem::LoadPresetIntoVoiceGroupItem(const Rect& r)
    : BasicItem("Import Preset", r)
{
}

void LoadPresetIntoVoiceGroupItem::doAction()
{
  Application::get().getHWUI()->setFocusAndMode(
      FocusAndMode(UIFocus::Sound, UIMode::Select, UIDetail::SoundSelectPresetForVoiceGroup));
}
