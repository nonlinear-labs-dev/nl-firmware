#include <Application.h>
#include <parameters/MacroControlParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MacroControlEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/EditMCInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/RenameMCLayout.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <http/UndoScope.h>
#include <groups/ParameterGroup.h>
#include "use-cases/EditBufferUseCases.h"
#include "parameters/ModulationRoutingParameter.h"

int MacroControlEditButtonMenu::s_lastAction = 0;

MacroControlEditButtonMenu::MacroControlEditButtonMenu(const Rect& rect)
    : super(rect)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onLocksChanged(mem_fun(this, &MacroControlEditButtonMenu::setup));
}

MacroControlEditButtonMenu::~MacroControlEditButtonMenu()
{
}

void MacroControlEditButtonMenu::setup()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  clear();

  auto param = getSelectedParameter();

  addButton("Rename", std::bind(&MacroControlEditButtonMenu::rename, this));
  addButton("Edit Info", std::bind(&MacroControlEditButtonMenu::editInfo, this));

  if(param->getParentGroup()->areAllParametersLocked())
    addButton("Unlock Group", std::bind(&MacroControlEditButtonMenu::unlockGroup, this));
  else
    addButton("Lock Group", std::bind(&MacroControlEditButtonMenu::lockGroup, this));

  addButton("Lock all", std::bind(&MacroControlEditButtonMenu::lockAll, this));

  if(eb->hasLocks(VoiceGroup::Global))
    addButton("Unlock all", std::bind(&MacroControlEditButtonMenu::unlockAll, this));

  auto mcGroup = eb->getParameterGroupByID({ "MCs", VoiceGroup::Global });
  mcGroup->onGroupChanged(mem_fun(this, &MacroControlEditButtonMenu::onGroupChanged));

  selectButton(s_lastAction);
}

void MacroControlEditButtonMenu::onGroupChanged()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto mcGroup = eb->getParameterGroupByID({ "MCs", VoiceGroup::Global });
  auto allParametersLocked = mcGroup->areAllParametersLocked();
  setItemTitle(2, allParametersLocked ? "Unlock Group" : "Lock Group");
}

void MacroControlEditButtonMenu::selectButton(size_t i)
{
  super::selectButton(i);
  s_lastAction = i;
}

void MacroControlEditButtonMenu::rename()
{
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(new RenameMCLayout());
}

void MacroControlEditButtonMenu::editInfo()
{
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(new EditMCInfoLayout());
}

void MacroControlEditButtonMenu::unlockGroup()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);
  auto param = getSelectedParameter();
  ebUseCases.unlockGroup(param->getParentGroup());
}

void MacroControlEditButtonMenu::lockGroup()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);
  auto param = getSelectedParameter();
  ebUseCases.lockGroup(param->getParentGroup());
}

void MacroControlEditButtonMenu::unlockAll()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);
  ebUseCases.unlockAllGroups();
}

void MacroControlEditButtonMenu::lockAll()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);
  ebUseCases.lockAllGroups();
}

Parameter* MacroControlEditButtonMenu::getSelectedParameter()
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto& eb = *Application::get().getPresetManager()->getEditBuffer();
  return eb.getSelected(vg);
}

Parameter* ModulationRouterMacroControlEditButtonMenu::getSelectedParameter()
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto& eb = *Application::get().getPresetManager()->getEditBuffer();
  auto selected = eb.getSelected(vg);
  if(auto modRouter = dynamic_cast<ModulationRoutingParameter*>(selected))
  {
    return modRouter->getTargetParameter();
  }

  return dynamic_cast<MacroControlParameter*>(selected);
}