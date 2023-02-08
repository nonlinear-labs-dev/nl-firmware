#pragma once
#include <proxies/hwui/controls/Label.h>
#include "ParameterId.h"

class RibbonParameter;
class Setting;

class LastTouchedRibbonLabel : public Label
{
 public:
  LastTouchedRibbonLabel(const ParameterId& id, const Rect& pos);

 private:
  void onBaseUnitModeChanged(const Setting* s);
  void onRibbonTouched(int ribbon);
  RibbonParameter* m_param;
  bool isInEditMode = false;
  bool isCurrentParamLastSelected = false;
};
