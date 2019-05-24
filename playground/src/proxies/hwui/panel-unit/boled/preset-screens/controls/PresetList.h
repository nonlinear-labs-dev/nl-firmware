#pragma once

#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetListBase.h>
#include <tools/Uuid.h>

class PresetList : public PresetListBase
{
  using super = PresetListBase;

 public:
  PresetList(const Rect &pos, bool showBankArrows);
  ~PresetList() override;

  virtual bool onButton(int i, bool down, ButtonModifiers) override;
  virtual void onRotary(int inc, ButtonModifiers modifiers) override;

  std::pair<size_t, size_t> getSelectedPosition() const override;

 private:
  void onBankSelectionChanged();
  void onBankChanged();
  void onEditBufferChanged();

  sigc::connection m_bankChangedConnection;
  Uuid m_uuidOfLastLoadedPreset;
};
