#include "LoadToPartPresetList.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <libundo/undo/Scope.h>
#include <libundo/undo/TransactionCreationScope.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <device-settings/DirectLoadSetting.h>
#include <device-settings/Settings.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <proxies/hwui/HWUIHelper.h>

LoadToPartPresetList::LoadToPartPresetList(const Rect& pos, bool showBankArrows)
    : PresetList(pos, showBankArrows)
{
}

LoadToPartPresetList::~LoadToPartPresetList() = default;

void LoadToPartPresetList::stepPresetSelection(int inc, PresetManager* pm, Bank* bank) const
{
  if(!HWUIHelper::isLoadToPartActive())
    PresetList::stepPresetSelection(inc, pm, bank);
  else
    stepPresetSelectionAndLoadPartOfNewPreset(inc, pm, bank);
}

void LoadToPartPresetList::stepPresetSelectionAndLoadPartOfNewPreset(int inc, PresetManager* pm, Bank* bank) const
{
  auto scope = pm->getUndoScope().startTransaction("Select Preset");
  while(inc < 0)
  {
    bank->selectPreviousPresetPart(scope->getTransaction());
    inc++;
  }

  while(inc > 0)
  {
    bank->selectNextPresetPart(scope->getTransaction());
    inc--;
  }
}

auto getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}