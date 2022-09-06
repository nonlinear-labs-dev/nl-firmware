#include "MacroControlSliderForCurrentModulationRouter.h"
#include "Application.h"
#include "parameters/ModulationRoutingParameter.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/MacroControlParameter.h>

MacroControlSliderForCurrentModulationRouter::MacroControlSliderForCurrentModulationRouter(const Rect& pos)
    : DotSlider(pos)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &MacroControlSliderForCurrentModulationRouter::subscribeToMacroFromSelected)), vg);
}

void MacroControlSliderForCurrentModulationRouter::subscribeToMacroFromSelected(Parameter* p)
{
  if(auto modRouter = dynamic_cast<ModulationRoutingParameter*>(p))
  {
    setParameter(modRouter->getTargetParameter());
  }
  else
  {
    nltools::Log::error("could not cast current parameter to router");
  }
}
