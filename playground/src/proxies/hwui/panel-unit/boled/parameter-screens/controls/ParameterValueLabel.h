#pragma once
#include <proxies/hwui/controls/Label.h>

class Parameter;

class ParameterValueLabel : public Label
{
 private:
  typedef Label super;

 public:
  ParameterValueLabel(const Parameter *param, const Rect &pos);

 private:
  void updateText(const Parameter *param);
};
