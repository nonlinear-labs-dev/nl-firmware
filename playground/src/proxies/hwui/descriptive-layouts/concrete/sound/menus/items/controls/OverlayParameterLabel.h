#pragma once
#include <proxies/hwui/controls/Label.h>
#include <parameters/Parameter.h>

class OverlayParameterLabel : public Label
{
 public:
  OverlayParameterLabel(const Rect& r, const Parameter* parameter);
  const Parameter* getParameter() const;

 private:
  void updateText();
  const Parameter* m_parameter;
  sigc::connection m_parameterChangedConnection;
};
