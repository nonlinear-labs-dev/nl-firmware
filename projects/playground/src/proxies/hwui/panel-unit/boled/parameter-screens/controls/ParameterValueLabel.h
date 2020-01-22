#pragma once

#include <proxies/hwui/controls/Label.h>
#include <sigc++/connection.h>

class Parameter;

class ParameterValueLabel : public Label
{
 private:
  typedef Label super;

 public:
  ParameterValueLabel(const Parameter *param, const Rect &pos);
  ~ParameterValueLabel();
  void updateParameter(const Parameter *param);

 private:
  void updateText(const Parameter *param);
  void updateVoiceGroup();
  sigc::connection m_connection;
  sigc::connection m_vgSelectionConnection;
};
