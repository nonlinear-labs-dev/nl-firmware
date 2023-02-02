#pragma once

#include "proxies/hwui/Layout.h"
#include "device-settings/BaseUnitUIDetail.h"

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
  Control* m_selectedRibbonsIndication = nullptr;

 private:
  PlayOrEditModeLayout(const PlayOrEditModeLayout& other);
  PlayOrEditModeLayout& operator=(const PlayOrEditModeLayout&);

  Control* m_pedalIndicationLower = nullptr;
  Control* m_ribbonLabelLower = nullptr;
  Control* m_ribbonModeLower = nullptr;
  Control* m_ribbonTouchedIndicatorLower = nullptr;
  Control* m_dottedLine = nullptr;

  void createRightSideControls();
  void createLowerLabels();
  void onRibbonSelectionChanged(const Setting* s);
  void onBaseUnitUIDetailChanged(const Setting* s);
  void onMacroMappingsChanged();

 protected:
  static bool isPedalMappedToCurrentLowerRibbon();
  static bool isPedalMappedToCurrentUpperRibbon();
  static BaseUnitUIDetail* getBaseUnitUIDetails();
};
