#include <parameters/ModulateableParameter.h>
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <presets/PresetManager.h>
#include <presets/PresetParameter.h>
#include <presets/EditBuffer.h>
#include <groups/MacroControlsGroup.h>
#include "RecallModulationSourceLabel.h"

RecallModulationSourceLabel::RecallModulationSourceLabel(Rect pos)
    : Label(pos)
{
  Glib::ustring text = "";
  if(auto modP
     = dynamic_cast<ModulateableParameter*>(Application::get().getPresetManager()->getEditBuffer()->getSelected()))
  {
    auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
    uint16_t id = MacroControlsGroup::modSrcToParamNumber(modP->getOriginalParameter()->getRecallModSource());
    if(auto mc = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(id, vg))
    {
      text = mc->getShortName();
    }
    if(text.empty())
      text = "[-]";
    setText(text);
    return;
  }
  setText(text);
}
