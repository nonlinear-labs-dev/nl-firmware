#pragma once

#include "proxies/hwui/controls/Label.h"
#include "proxies/hwui/controls/labels/LabelStyleable.h"
#include "parameters/Parameter.h"
#include "ControlWithChildren.h"

class Application;
class Parameter;
class Setting;

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

class HardwareSourceSendParameter;
class PhysicalControlParameter;

class PhysicalControlValueLabel : public ControlWithChildren
{
 public:
  explicit PhysicalControlValueLabel(const Rect& rect);
  bool redraw(FrameBuffer& fb) override;
  void setHighlight(bool isHighlight) override;

 private:
  void onModifiersChanged(ButtonModifiers mods);
  void onParameterSelectionHappened(const Parameter* old, Parameter* newP);
  void onSendChanged(const Parameter* p);
  void onHWChanged(const Parameter* p);

  LabelStyleable* m_localEnabledLabel = nullptr;
  LabelStyleable* m_localDisabledLabelSnd = nullptr;
  LabelStyleable* m_localDisabledLabelRcv = nullptr;

  HardwareSourceSendParameter* m_snd = nullptr;
  PhysicalControlParameter* m_hw = nullptr;

  sigc::connection m_sndChanged;
  sigc::connection m_hwChanged;

  bool m_isLocalEnabled = true;
};

class HardwareSourceCCLabel : public LabelStyleable
{
 public:
  explicit HardwareSourceCCLabel(const Rect& e);

  void onParameterSelectionHappened(const Parameter* old, const Parameter* newP);
  void onSettingsChanged(const Setting* changed);

 private:
  sigc::connection m_settingConnection;
};
