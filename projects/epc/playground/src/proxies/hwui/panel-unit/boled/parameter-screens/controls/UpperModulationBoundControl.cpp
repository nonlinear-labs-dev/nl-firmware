#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UpperModulationBoundControl.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UpperModulationBoundLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UpperModulationBoundSlider.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/MacroControlParameter.h>
#include <math.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <libundo/undo/Scope.h>
#include <proxies/hwui/HWUI.h>
#include "use-cases/ModParameterUseCases.h"

UpperModulationBoundControl::UpperModulationBoundControl(const Rect &r)
    : super(r)
{
  auto height = r.getHeight() / 2;
  addControl(new UpperModulationBoundSlider(Rect(0, 0, r.getWidth(), height)));
  addControl(new UpperModulationBoundLabel(Rect(0, height, r.getWidth(), height)));
}

bool UpperModulationBoundControl::onRotary(int inc, ButtonModifiers modifiers)
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();

  if(auto modulatedParam = dynamic_cast<ModulateableParameter *>(editBuffer->getSelected(vg)))
  {
    ModParameterUseCases useCase(modulatedParam);
    useCase.incUpperModulationBound(inc, modifiers[FINE]);
    return true;
  }
  return false;
}
