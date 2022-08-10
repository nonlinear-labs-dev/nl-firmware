#include "RenameMCLayout.h"
#include "Application.h"
#include "use-cases/MacroControlParameterUseCases.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/MacroControlParameter.h>
#include <proxies/hwui/HWUI.h>

RenameMCLayout::RenameMCLayout()
    : super()
{
}

void RenameMCLayout::commit(const Glib::ustring &newName)
{
  if(auto p = getMacroControl())
  {
    MacroControlParameterUseCases mcUseCase(p);
    mcUseCase.setName(newName);
  }
}

Glib::ustring RenameMCLayout::getInitialText() const
{
  if(auto p = getMacroControl())
  {
    return p->getGivenName();
  }

  return "";
}

MacroControlParameter *RenameMCLayout::getMacroControl() const
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  return dynamic_cast<MacroControlParameter *>(Application::get().getPresetManager()->getEditBuffer()->getSelected(vg));
}
