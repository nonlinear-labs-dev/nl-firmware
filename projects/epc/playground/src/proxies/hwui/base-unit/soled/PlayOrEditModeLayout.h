#pragma once

#include "proxies/hwui/Layout.h"

class Label;
class Setting;
class Parameter;

class PlayOrEditModeLayout : public Layout
{
 private:
  typedef Layout super;

 public:
  PlayOrEditModeLayout();
  ~PlayOrEditModeLayout() override;

 protected:
  void onParameterSelectionChanged(const Parameter* old, Parameter* newP);

 private:
  PlayOrEditModeLayout(const PlayOrEditModeLayout& other);
  PlayOrEditModeLayout& operator=(const PlayOrEditModeLayout&);

  Control* m_pedalIndicationLower = nullptr;
  Control* m_ribbonLabelLower = nullptr;
  void createLowerLabels();
  void onRibbonSelectionChanged(const Setting* s);
  void onMacroMappingsChanged();

 protected:
  bool isPedalMappedToCurrentLowerRibbon();
  bool isPedalMappedToCurrentUpperRibbon();
};
