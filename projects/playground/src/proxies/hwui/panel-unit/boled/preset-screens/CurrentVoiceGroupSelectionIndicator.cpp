#include "CurrentVoiceGroupSelectionIndicator.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <sigc++/sigc++.h>

inline static Glib::ustring currentVoiceGroupToString()
{
  auto currentVG = Application::get().getPresetManager()->getEditBuffer()->getCurrentHWUIVoiceGroup();
  return toString(currentVG);
}

CurrentVoiceGroupSelectionIndicator::CurrentVoiceGroupSelectionIndicator(const Rect &r)
    : LabelRegular8(currentVoiceGroupToString(), r)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onCurrentVoiceGroupChanged(sigc::hide(sigc::mem_fun(this, &CurrentVoiceGroupSelectionIndicator::focusChanged)));
  eb->onPresetLoaded(sigc::mem_fun(this, &CurrentVoiceGroupSelectionIndicator::focusChanged));
  eb->onSoundTypeChanged(sigc::hide(sigc::mem_fun(this, &CurrentVoiceGroupSelectionIndicator::focusChanged)));
}

void CurrentVoiceGroupSelectionIndicator::focusChanged()
{
  if(Application::get().getPresetManager()->getEditBuffer()->getType() != SoundType::Single)
    setText({ currentVoiceGroupToString(), 0 });
  else
    setText({ "", 0 });
}
