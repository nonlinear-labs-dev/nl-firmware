#pragma once
#include <nltools/Types.h>
#include "PresetList.h"

class LoadToPartPresetList : public PresetList
{
  using super = PresetList;

 public:
  LoadToPartPresetList(const Rect& pos, bool showBankArrows);
  ~LoadToPartPresetList() override;

 protected:
  void stepPresetSelection(int inc, PresetManager* pm, Bank* bank) const override;

  static bool shouldLoadPartDirect();
  void stepPresetSelectionAndLoadPartOfNewPreset(int inc, PresetManager* pm, Bank* bank) const;
  void stepPresetSelectionLoadToPart(int inc, PresetManager* pm, Bank* bank) const;
  void onStepPresetSelectionLoadToPart(const Preset* newSelection, Bank* bank, PresetManager* pm,
                                       bool selectNext) const;

  void onSinglePresetSelecton(const Preset* preset, PresetManager* pm, Bank* bank) const;
  void onDualPresetSelection(const Preset* preset, VoiceGroup loadPart, Bank* bank, PresetManager* pm) const;
};