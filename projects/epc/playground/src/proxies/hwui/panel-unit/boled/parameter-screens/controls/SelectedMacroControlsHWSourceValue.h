#pragma once

#include "proxies/hwui/controls/Label.h"
#include <ParameterId.h>
#include <sigc++/connection.h>

class Application;
class Parameter;

class SelectedMacroControlsHWSourceValue : public Label
{
 private:
  typedef Label super;

 public:
  explicit SelectedMacroControlsHWSourceValue(const Rect &rect);
  ~SelectedMacroControlsHWSourceValue() override;

  SelectedMacroControlsHWSourceValue(const SelectedMacroControlsHWSourceValue &other) = delete;
  SelectedMacroControlsHWSourceValue &operator=(const SelectedMacroControlsHWSourceValue &) = delete;

 private:
  void onModifiersChanged();
  void onParameterSelected(Parameter *newOne);
  void onMCChanged(const Parameter *param);
  void updateText(const Parameter *param);
  void setSuffixFontColor(FrameBuffer &fb) const override;
  ParameterId getHWSourceID(const Parameter *param) const;

  ParameterId m_hwParamID;
  sigc::connection m_mcChanged;
  sigc::connection m_hwChanged;
};

class SelectedModRouterMacroControlValue : public Label
{
 private:
  typedef Label super;

 public:
  explicit SelectedModRouterMacroControlValue(const Rect &rect);
  ~SelectedModRouterMacroControlValue() override;

  SelectedModRouterMacroControlValue(const SelectedModRouterMacroControlValue &other) = delete;
  SelectedModRouterMacroControlValue &operator=(const SelectedModRouterMacroControlValue &) = delete;

 private:
  void onModifiersChanged();
  void onParameterSelected(Parameter *newOne);
  void updateText(const Parameter *param);
  void setSuffixFontColor(FrameBuffer &fb) const override;

  sigc::connection m_mcChanged;
  ParameterId m_mcId;
};
