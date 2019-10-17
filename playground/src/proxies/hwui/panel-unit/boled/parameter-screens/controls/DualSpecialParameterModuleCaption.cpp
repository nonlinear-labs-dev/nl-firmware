#include "DualSpecialParameterModuleCaption.h"

DualSpecialParameterModuleCaption::DualSpecialParameterModuleCaption(const Rect &pos)
    : ModuleCaption(pos)
{
}

void DualSpecialParameterModuleCaption::updateText(Parameter *newOne)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto type = eb->getType();
  auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();

  if(type == SoundType::Single)
  {
    setText({ "Single", 0 });
  }
  else
  {
    if(dynamic_cast<const SplitPointParameter *>(eb->getSelected()))
      setText({ toString(type) + " I+II", 0 });
    else
      setText({ toString(type) + " " + toString(vg), 0 });
  }
}
