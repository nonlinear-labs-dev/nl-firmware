#pragma once
#include "proxies/hwui/controls/Label.h"
#include "ParameterId.h"
#include "parameters/RibbonParameter.h"

class RibbonModeLabel : public Label
{
 public:
  RibbonModeLabel(const ParameterId &paramID, const Rect &rect);
  StringAndSuffix getText() const override;

 private:
  RibbonParameter *m_param;
};
