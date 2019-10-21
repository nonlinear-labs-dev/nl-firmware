#pragma once
#include <proxies/hwui/controls/Label.h>

class Parameter;

class ParameterValueLabel : public Label
{
 private:
  typedef Label super;

 public:
  ParameterValueLabel(const Parameter *param, const Rect &pos);
  ~ParameterValueLabel();
  void updateParameter(const Parameter* param);
 private:
  void updateText(const Parameter *param);
  sigc::connection m_connection;
};
