#pragma once

#include "PlayOrEditModeLayout.h"

class Label;
class Setting;

class PlayModeLayout : public PlayOrEditModeLayout
{
 private:
  typedef PlayOrEditModeLayout super;

 public:
  PlayModeLayout();
  ~PlayModeLayout() override;

 private:
  void createUpperLabels();
  void onMacroControlMappingsChanged();
  void onRibbonSelectionChanged(const Setting* s);

  Control* m_pedalSymbol = nullptr;
  Control* m_ribbonLabel = nullptr;
};
