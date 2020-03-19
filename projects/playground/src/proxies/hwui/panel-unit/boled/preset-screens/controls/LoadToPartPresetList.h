#pragma once
#include "PresetList.h"

class LoadToPartPresetList : public PresetList
{
  using super = PresetList;

 public:
  LoadToPartPresetList(const Rect& pos, bool showBankArrows);
  ~LoadToPartPresetList() override;
  void onRotary(int inc, ButtonModifiers modifiers) override;
};