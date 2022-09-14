#include "EditMCInfoLayout.h"
#include "Application.h"
#include "use-cases/MacroControlParameterUseCases.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/MacroControlParameter.h>
#include <proxies/hwui/HWUI.h>

EditMCInfoLayout::EditMCInfoLayout()
    : super()
{
}

void EditMCInfoLayout::commit(const Glib::ustring &newName)
{
  if(auto p = getMacroControl())
  {
    MacroControlParameterUseCases useCase(p);
    useCase.setInfo(newName);
  }
}

Glib::ustring EditMCInfoLayout::getInitialText() const
{
  if(auto p = getMacroControl())
  {
    return p->getInfo();
  }

  return "";
}

MacroControlParameter *EditMCInfoLayout::getMacroControl() const
{
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
  return dynamic_cast<MacroControlParameter *>(
      Application::get().getPresetManager()->getEditBuffer()->getSelected(currentVG));
}
