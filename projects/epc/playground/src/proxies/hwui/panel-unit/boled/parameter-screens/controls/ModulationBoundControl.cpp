#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationBoundControl.h>
#include <Application.h>
#include <proxies/hwui/buttons.h>
#include <parameters/ModulateableParameter.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <libundo/undo/Scope.h>
#include <proxies/hwui/HWUI.h>
#include "use-cases/ModParameterUseCases.h"

bool ModulationBoundControl::onButton(Buttons i, bool down, ButtonModifiers)
{
  switch(i)
  {
    case Buttons::BUTTON_DEFAULT:
      if(down)
      {
        auto eb = Application::get().getPresetManager()->getEditBuffer();
        auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
        if(auto modulatedParam = dynamic_cast<ModulateableParameter *>(eb->getSelected(vg)))
        {
          ModParameterUseCases useCase(modulatedParam);
          useCase.setModulationAmount(0);
        }
        return true;
      }
      break;
  }
  return false;
}
