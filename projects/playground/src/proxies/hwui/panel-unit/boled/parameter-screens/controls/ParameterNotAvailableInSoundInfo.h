#pragma once
#include "MultiLineLabel.h"
#include <sigc++/connection.h>

class Parameter;

class ParameterNotAvailableInSoundInfo : public ControlWithChildren
{
 public:
  ParameterNotAvailableInSoundInfo(const Rect& r, ParameterLayout2* parent);
  void setVisible(bool b) override;
  void init();

 protected:
  void setBackgroundColor(FrameBuffer& fb) const override;

 private:
  void onSelectionChanged(const Parameter* old, const Parameter* newParam);
  void onSoundTypeChanged();

  ParameterLayout2* m_parent;

  sigc::connection m_parameterSelectionConnection;
  sigc::connection m_sountTypeConnection;
};
