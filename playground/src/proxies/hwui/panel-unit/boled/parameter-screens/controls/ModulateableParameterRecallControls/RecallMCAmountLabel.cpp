#include "RecallMCAmountLabel.h"
#include <parameters/ModulateableParameter.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/PresetParameter.h>
#include <presets/EditBuffer.h>
#include <groups/MacroControlsGroup.h>

RecallMCAmountLabel::RecallMCAmountLabel(Rect pos)
    : Label(pos)
{
  if(auto modP
     = dynamic_cast<ModulateableParameter*>(Application::get().getPresetManager()->getEditBuffer()->getSelected()))
  {
    if(auto originalParam = modP->getOriginalParameter())
    {
      setText(modP->stringizeModulationAmount(originalParam->getModulationAmount()));
      return;
    }
  }
  setText("");
}