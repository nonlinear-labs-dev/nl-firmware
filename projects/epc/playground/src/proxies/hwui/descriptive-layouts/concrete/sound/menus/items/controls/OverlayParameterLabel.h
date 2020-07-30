#pragma once

#include <proxies/hwui/controls/Label.h>
#include <sigc++/connection.h>

class Parameter;

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
