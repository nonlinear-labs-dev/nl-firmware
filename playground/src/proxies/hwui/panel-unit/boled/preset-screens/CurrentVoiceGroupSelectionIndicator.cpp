#include "CurrentVoiceGroupSelectionIndicator.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

inline static Glib::ustring currentVoiceGroupToString()
{
  auto currentVG = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  return toString(currentVG);
}

CurrentVoiceGroupSelectionIndicator::CurrentVoiceGroupSelectionIndicator(const Rect &r)
    : SymbolLabel(currentVoiceGroupToString(), r)
{
  Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged(
      sigc::mem_fun(this, &CurrentVoiceGroupSelectionIndicator::focusChanged));
}

void CurrentVoiceGroupSelectionIndicator::focusChanged()
{
  if(Application::get().getPresetManager()->getEditBuffer()->getType() != SoundType::Single)
    setText({ currentVoiceGroupToString(), 0 });
}