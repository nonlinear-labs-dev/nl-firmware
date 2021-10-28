#pragma once

#include "proxies/hwui/controls/Label.h"
#include "parameters/Parameter.h"
#include "ControlWithChildren.h"

class Application;
class Parameter;

class SelectedParameterValue : public Label
{
 private:
  typedef Label super;

 public:
  explicit SelectedParameterValue(const Rect& rect);
  ~SelectedParameterValue() override;

 protected:
  virtual void onParameterSelected(Parameter* parameter);
  virtual void onParamValueChanged(const Parameter* param);

  void setSuffixFontColor(FrameBuffer& fb) const override;
  virtual void onModifiersChanged(ButtonModifiers mods);

  void onVoiceGroupSelectionChanged(VoiceGroup v);
  void onSoundTypeChanged();

  bool redraw(FrameBuffer& fb) override;

 private:
  sigc::connection m_paramValueConnection;
};

class LabelStyleable;
class HardwareSourceSendParameter;
class PhysicalControlParameter;

class PhysicalControlValueLabel : public ControlWithChildren
{
 public:
  explicit PhysicalControlValueLabel(const Rect& rect);
  bool redraw(FrameBuffer& fb) override;

 private:
  void onParameterSelectionHappened(const Parameter* old, Parameter* newP);
  void onSendChanged(const Parameter* p);
  void onHWChanged(const Parameter* p);

  LabelStyleable* m_localEnabledLabel = nullptr;
  LabelStyleable* m_localDisabledLabelSnd = nullptr;
  LabelStyleable* m_localDisabledLabelRcv = nullptr;

  HardwareSourceSendParameter* m_snd;
  PhysicalControlParameter* m_hw;

  sigc::connection m_sndChanged;
  sigc::connection m_hwChanged;

  bool m_isLocalEnabled = true;
};
