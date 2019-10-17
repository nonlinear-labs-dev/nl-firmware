#include "LoadVoiceGroupPresetList.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "proxies/hwui/HWUI.h"

LoadVoiceGroupPresetList::LoadVoiceGroupPresetList(const Point &p)
    : GenericPresetList(p)
{
  signalChanged();
}

void LoadVoiceGroupPresetList::action()
{
  if(auto preset = getPresetAtSelected())
  {
    Application::get().getPresetManager()->getEditBuffer()->undoableLoad(preset,
                                                                         Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection());
    Application::get().getHWUI()->setFocusAndMode(UIDetail::Init);
  }
}
