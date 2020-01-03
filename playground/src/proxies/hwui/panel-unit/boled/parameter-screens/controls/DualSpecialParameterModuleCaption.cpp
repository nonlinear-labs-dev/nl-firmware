#include "DualSpecialParameterModuleCaption.h"
#include <proxies/hwui/HWUI.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

DualSpecialParameterModuleCaption::DualSpecialParameterModuleCaption(const Rect &pos)
    : ModuleCaption(pos)
{
}

void DualSpecialParameterModuleCaption::updateText(Parameter *newOne)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto type = eb->getType();
  auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();

  if(type == SoundType::Single)
  {
    setText({ "Single", 0 });
  }
  else
  {
    setText({ "Part " + toString(vg), 0 });
  }
}
