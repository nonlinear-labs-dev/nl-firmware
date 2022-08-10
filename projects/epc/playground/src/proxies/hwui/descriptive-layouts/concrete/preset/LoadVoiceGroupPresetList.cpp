#include "LoadVoiceGroupPresetList.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "proxies/hwui/HWUI.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/EditBufferUseCases.h"

LoadVoiceGroupPresetList::LoadVoiceGroupPresetList(const Point &p)
    : GenericPresetList(p)
{
  signalChanged();
}

void LoadVoiceGroupPresetList::action()
{
  if(auto preset = getPresetAtSelected())
  {
    EditBufferUseCases useCase(*Application::get().getPresetManager()->getEditBuffer());
    SettingsUseCases useCases(*Application::get().getSettings());
    useCase.loadToPart(preset, VoiceGroup::I, Application::get().getVGManager()->getCurrentVoiceGroup());
    useCases.setFocusAndMode(UIDetail::Init);
  }
}
