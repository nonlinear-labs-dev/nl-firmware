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
  if(!isLoadToPartActive())
    PresetList::stepPresetSelection(inc, pm, bank);
  else
    stepPresetSelectionAndLoadPartOfNewPreset(inc, pm, bank);
}

void LoadToPartPresetList::stepPresetSelectionAndLoadPartOfNewPreset(int inc, PresetManager* pm, Bank* bank) const
{
  while(inc > 0)
  {
    stepPresetSelectionLoadToPart(inc--, pm, bank);
  }

  while(inc < 0)
  {
    stepPresetSelectionLoadToPart(inc++, pm, bank);
  }
}

void LoadToPartPresetList::stepPresetSelectionLoadToPart(int inc, PresetManager* pm, Bank* bank) const
{
  const auto selectNext = inc > 0;

  if(bank->getSelectedPreset())
  {
    if(auto preset = selectNext ? bank->getNextPreset() : bank->getPreviousPreset())
    {
      onStepPresetSelectionLoadToPart(preset, bank, pm, selectNext);
    }
  }
  else
  {
    if(auto preset = selectNext ? bank->getFirstPreset() : bank->getLastPreset())
    {
      onStepPresetSelectionLoadToPart(preset, bank, pm, selectNext);
    }
  }
}

void LoadToPartPresetList::onStepPresetSelectionLoadToPart(const Preset* newSelection, Bank* bank, PresetManager* pm,
                                                           bool selectNext) const
{
  if(!newSelection)
    return;

  auto isDual = newSelection->getType() != SoundType::Single;
  if(isDual)
  {
    auto loadedPart = pm->getEditBuffer()->getLoadedPartOfPreset(newSelection);
    auto isPartlyLoaded = loadedPart.has_value();

    if(isPartlyLoaded)
    {
      auto currentLoadedPresetPart = loadedPart.value();
      if(currentLoadedPresetPart == VoiceGroup::I)
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

auto getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}

LoadToPartPresetList::DualPresetSelection LoadToPartPresetList::getToLoad(int inc, Bank* bank, PresetManager* pm) const
{
  auto selectNext = inc > 0;
  DualPresetSelection ret {};
  Preset* currentSelectedPreset = nullptr;

  if(auto selectedPreset = bank->getSelectedPreset())
  {
    currentSelectedPreset = selectedPreset;

    auto handlePresetStep = [](bool next, Preset* selected, Bank* bank) -> DualPresetSelection {
      auto eb = getEditBuffer();
      if(selected->getType() != SoundType::Single)
      {
        auto selectedDual = selected;
        if(auto loadedPart = eb->getLoadedPartOfPreset(selectedDual))
        {
          if(next && loadedPart.value() == VoiceGroup::I)
          {
            return DualPresetSelection(selectedDual, VoiceGroup::II);
          }
          else if(next && loadedPart.value() == VoiceGroup::II)
          {
            return DualPresetSelection(bank->getPresetAt(bank->getPresetPosition(selectedDual) + 1), VoiceGroup::I);
          }
          else if(!next && loadedPart.value() == VoiceGroup::I)
          {
            auto prev = bank->getPresetAt(bank->getPresetPosition(selectedDual) - 1);
            return DualPresetSelection(prev, prev->getType() != SoundType::Single ? VoiceGroup::II : VoiceGroup::I);
          }
          else if(!next && loadedPart.value() == VoiceGroup::II)
          {
            return DualPresetSelection(selectedDual, VoiceGroup::I);
          }
        }
        else
        {
          if(next)
          {
            return DualPresetSelection(selected, VoiceGroup::I);
          }
          else
          {
            return DualPresetSelection(selected, VoiceGroup::II);
          }
        }
      }
    };

    while(inc > 0)
    {
      ret = handlePresetStep(true, currentSelectedPreset, bank);
      inc--;
    }

    while(inc < 0)
    {

      inc++;
    }
  }
  else
  {
  }

  return LoadToPartPresetList::DualPresetSelection();
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

bool LoadToPartPresetList::isDirectLoadActive()
{
  return Application::get().getSettings()->getSetting<DirectLoadSetting>()->get();
}

bool LoadToPartPresetList::isLoadToPartActive()
{
  return Application::get().getSettings()->getSetting<LoadToPartSetting>()->get();
}