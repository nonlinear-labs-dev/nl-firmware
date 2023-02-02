#pragma once
#include <proxies/hwui/controls/Label.h>
#include "ParameterId.h"

class RibbonParameter;

class LastTouchedRibbonLabel : public Label
{
 public:
  LastTouchedRibbonLabel(const ParameterId& id, const Rect& pos);

 private:
  void onRibbonTouched(int ribbon);
  RibbonParameter* m_param;
};
