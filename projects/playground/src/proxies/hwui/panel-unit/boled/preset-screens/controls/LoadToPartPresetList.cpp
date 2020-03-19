#include "LoadToPartPresetList.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <device-settings/LoadToPartSetting.h>
#include <device-settings/DirectLoadSetting.h>
#include <device-settings/Settings.h>
#include <presets/Bank.h>
#include <presets/Preset.h>

LoadToPartPresetList::LoadToPartPresetList(const Rect& pos, bool showBankArrows)
    : PresetList(pos, showBankArrows)
{
}
LoadToPartPresetList::~LoadToPartPresetList() = default;

void LoadToPartPresetList::stepPresetSelection(int inc, PresetManager* pm, Bank* bank) const
{
  if(!shouldLoadPartDirect())
    PresetList::stepPresetSelection(inc, pm, bank);
  else
    stepPresetSelectionAndLoadPartOfNewPreset(inc, pm, bank);
}

bool LoadToPartPresetList::shouldLoadPartDirect()
{
  auto isLoadToPart = Application::get().getSettings()->getSetting<LoadToPartSetting>()->get();
  auto isDirectLoad = Application::get().getSettings()->getSetting<DirectLoadSetting>()->get();
  return isLoadToPart && isDirectLoad;
}

void LoadToPartPresetList::stepPresetSelectionAndLoadPartOfNewPreset(int inc, PresetManager* pm, Bank* bank) const
{

  while(inc > 0)
  {
    stepPresetSelectionLoadToPart(inc++, pm, bank);
  }

  while(inc < 0)
  {
    stepPresetSelectionLoadToPart(inc--, pm, bank);
  }
}

void LoadToPartPresetList::stepPresetSelectionLoadToPart(int inc, PresetManager* pm, Bank* bank) const
{
  const auto selectNext = inc > 0;

  if(bank->getSelectedPreset())
  {
    auto preset = selectNext ? bank->getNextPreset() : bank->getPreviousPreset();
    onStepPresetSelectionLoadToPart(preset, bank, pm, selectNext);
  }
}

void LoadToPartPresetList::onStepPresetSelectionLoadToPart(const Preset* newSelection, Bank* bank, PresetManager* pm,
                                                           bool selectNext) const
{
  auto isDual = newSelection->getType() != SoundType::Single;
  if(isDual)
  {
    auto loadedPart = pm->getEditBuffer()->getLoadedPartOfPreset(newSelection);
    auto isPartlyLoaded = loadedPart.has_value();

    if(isPartlyLoaded)
    {
      auto currentLoadedPresetPart = loadedPart.value();
      if(selectNext)
      {
      }
      else
      {
      }
    }
    else
    {
      onDualPresetSelection(newSelection, selectNext ? VoiceGroup::I : VoiceGroup::II, bank, pm);
    }
  }
  else
  {
    onSinglePresetSelecton(newSelection, pm, bank);
  }
}

void LoadToPartPresetList::onSinglePresetSelecton(const Preset* preset, PresetManager* pm, Bank* bank) const
{
  auto scope = pm->getUndoScope().startContinuousTransaction(
      bank, "Load Preset into Part %s", toString(Application::get().getHWUI()->getCurrentVoiceGroup()));
  bank->selectNextPreset(scope->getTransaction());
}

void LoadToPartPresetList::onDualPresetSelection(const Preset* preset, VoiceGroup loadPart, Bank* bank,
                                                 PresetManager* pm) const
{
  auto to = Application::get().getHWUI()->getCurrentVoiceGroup();
  auto scope = pm->getUndoScope().startContinuousTransaction(bank, "Load Dual Preset Part %s into Part %s",
                                                             toString(loadPart), toString(to));
  bank->selectNextPreset(scope->getTransaction());
  pm->getEditBuffer()->undoableLoadPresetPartIntoPart(scope->getTransaction(), preset, loadPart, to);
}