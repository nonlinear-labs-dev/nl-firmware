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

  void stepPresetSelectionAndLoadPartOfNewPreset(int inc, PresetManager* pm, Bank* bank) const;
};