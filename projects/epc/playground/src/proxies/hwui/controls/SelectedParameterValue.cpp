#include <Application.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/SelectedParameterValue.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/FrameBuffer.h>
#include <sigc++/sigc++.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>
#include <proxies/hwui/controls/labels/LabelStyleable.h>
#include <nltools/StringTools.h>
#include <device-settings/midi/mappings/RibbonCCMapping.h>
#include <device-settings/midi/mappings/PedalCCMapping.h>
#include <device-settings/midi/mappings/AftertouchCCMapping.h>
#include <device-settings/midi/mappings/BenderCCMapping.h>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/HardwareSourceSendParameter.h>
#include "parameter_declarations.h"

SelectedParameterValue::SelectedParameterValue(const Rect &rect)
    : super(rect)
{
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedParameterValue::onParameterSelected)),
      Application::get().getVGManager()->getCurrentVoiceGroup());

  Application::get().getHWUI()->onModifiersChanged(sigc::mem_fun(this, &SelectedParameterValue::onModifiersChanged));

  Application::get().getVGManager()->onCurrentVoiceGroupChanged(
      sigc::mem_fun(this, &SelectedParameterValue::onVoiceGroupSelectionChanged));

  Application::get().getPresetManager()->getEditBuffer()->onSoundTypeChanged(
      sigc::hide(sigc::mem_fun(this, &SelectedParameterValue::onSoundTypeChanged)), false);
}

SelectedParameterValue::~SelectedParameterValue()
{
}

void SelectedParameterValue::onModifiersChanged(ButtonModifiers mods)
{
  onParamValueChanged(Application::get().getPresetManager()->getEditBuffer()->getSelected(
      Application::get().getVGManager()->getCurrentVoiceGroup()));
}

void SelectedParameterValue::onParameterSelected(Parameter *parameter)
{
  m_paramValueConnection.disconnect();

  if(parameter)
  {
    m_paramValueConnection
        = parameter->onParameterChanged(sigc::mem_fun(this, &SelectedParameterValue::onParamValueChanged));

    setVisible(!parameter->isDisabled());
  }
}

void SelectedParameterValue::onParamValueChanged(const Parameter *param)
{
  setDirty();
}

bool SelectedParameterValue::redraw(FrameBuffer &fb)
{
  auto amount = Application::get()
                    .getPresetManager()
                    ->getEditBuffer()
                    ->getSelected(Application::get().getVGManager()->getCurrentVoiceGroup())
                    ->getDisplayString();

  if(Application::get().getHWUI()->isModifierSet(ButtonModifier::FINE))
  {
    setText({ amount, " F" });
  }
  else
  {
    setText(StringAndSuffix { amount });
  }

  return super::redraw(fb);
}

void SelectedParameterValue::setSuffixFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C128);
}

void SelectedParameterValue::onVoiceGroupSelectionChanged(VoiceGroup v)
{
  setDirty();
}

void SelectedParameterValue::onSoundTypeChanged()
{
  auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected(
      Application::get().getVGManager()->getCurrentVoiceGroup());
  setVisible(!selected->isDisabled());
}

PhysicalControlValueLabel::PhysicalControlValueLabel(const Rect &rect)
    : ControlWithChildren(rect)
{
  const auto &pos = rect;
  auto hW = pos.getWidth() / 2;
  auto left = Rect(0, 0, hW, pos.getHeight());
  auto right = Rect(hW, 0, hW, pos.getHeight());

  LabelStyle style { .size = FontSize::Size9,
                     .decoration = FontDecoration::Regular,
                     .justification = Font::Justification::Center,
                     .backgroundColor = FrameBufferColors::Transparent };

  m_localEnabledLabel = addControl(new LabelStyleable({ 0, 0, pos.getWidth(), pos.getHeight() }));
  m_localDisabledLabelSnd = addControl(new LabelStyleable(left));
  m_localDisabledLabelRcv = addControl(new LabelStyleable(right));
  m_localEnabledLabel->setLabelStyle(style);
  m_localDisabledLabelSnd->setLabelStyle(style);
  m_localDisabledLabelRcv->setLabelStyle(style);
  m_localEnabledLabel->setHighlight(true);

  auto settings = Application::get().getSettings();
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onSelectionChanged(sigc::mem_fun(this, &PhysicalControlValueLabel::onParameterSelectionHappened),
                         VoiceGroup::Global);
}

void PhysicalControlValueLabel::setHighlight(bool isHighlight)
{
}

void PhysicalControlValueLabel::onParameterSelectionHappened(const Parameter *old, Parameter *newP)
{
  if(auto hw = dynamic_cast<PhysicalControlParameter *>(newP))
  {
    m_hw = hw;
    m_snd = hw->getSendParameter();
    m_localDisabledLabelSnd->setHighlight(false);
    m_localDisabledLabelRcv->setHighlight(true);
  }
  else if(auto snd = dynamic_cast<HardwareSourceSendParameter *>(newP))
  {
    m_hw = snd->getSiblingParameter();
    m_snd = snd;
    m_localDisabledLabelSnd->setHighlight(true);
    m_localDisabledLabelRcv->setHighlight(false);
  }

  m_hwChanged.disconnect();
  m_sndChanged.disconnect();

  m_sndChanged = m_snd->onParameterChanged(sigc::mem_fun(this, &PhysicalControlValueLabel::onSendChanged), true);
  m_hwChanged = m_hw->onParameterChanged(sigc::mem_fun(this, &PhysicalControlValueLabel::onHWChanged), true);
}

bool PhysicalControlValueLabel::redraw(FrameBuffer &fb)
{
  ControlWithChildren::drawBackground(fb);

  m_localEnabledLabel->setVisible(m_isLocalEnabled);
  m_localDisabledLabelSnd->setVisible(!m_isLocalEnabled);
  m_localDisabledLabelRcv->setVisible(!m_isLocalEnabled);

  return ControlWithChildren::redraw(fb);
}

void PhysicalControlValueLabel::onSendChanged(const Parameter *p)
{
  if(auto send = dynamic_cast<const HardwareSourceSendParameter *>(p))
  {
    m_isLocalEnabled = send->isLocalEnabled();
    auto str = send->getDisplayString();
    auto shorter = nltools::string::removeCharacters(str, { '%', ' ' });
    m_localDisabledLabelSnd->setText({ shorter });
    ControlWithChildren::setDirty();
  }
}

void PhysicalControlValueLabel::onHWChanged(const Parameter *p)
{
  if(auto hw = dynamic_cast<const PhysicalControlParameter *>(p))
  {
    m_isLocalEnabled = hw->isLocalEnabled();
    auto str = hw->getDisplayString();
    auto shorter = nltools::string::removeCharacters(str, { '%', ' ' });
    m_localEnabledLabel->setText({ str });
    m_localDisabledLabelRcv->setText({ shorter });
    ControlWithChildren::setDirty();
  }
}

HardwareSourceCCLabel::HardwareSourceCCLabel(const Rect &e)
    : LabelStyleable(e,
                     { .size = FontSize::Size8,
                       .decoration = FontDecoration::Regular,
                       .justification = Font::Justification::Left,
                       .backgroundColor = FrameBufferColors::Transparent })
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onSelectionChanged(sigc::mem_fun(this, &HardwareSourceCCLabel::onParameterSelectionHappened), VoiceGroup::Global);
}

Setting *getMappingSetting(int id, Settings *settings)
{
  switch(id)
  {
    case C15::PID::Pedal_1:
      return settings->getSetting<PedalCCMapping<1>>();
    case C15::PID::Pedal_2:
      return settings->getSetting<PedalCCMapping<2>>();
    case C15::PID::Pedal_3:
      return settings->getSetting<PedalCCMapping<3>>();
    case C15::PID::Pedal_4:
      return settings->getSetting<PedalCCMapping<4>>();
    case C15::PID::Ribbon_1:
      return settings->getSetting<RibbonCCMapping<1>>();
    case C15::PID::Ribbon_2:
      return settings->getSetting<RibbonCCMapping<2>>();
    case C15::PID::Bender:
      return settings->getSetting<BenderCCMapping>();
    case C15::PID::Aftertouch:
      return settings->getSetting<AftertouchCCMapping>();
  }
  return nullptr;
}

void HardwareSourceCCLabel::onParameterSelectionHappened(const Parameter *old, const Parameter *newP)
{
  auto settings = Application::get().getSettings();
  const Parameter *param = newP;
  if(auto send = dynamic_cast<const HardwareSourceSendParameter *>(newP))
  {
    param = send->getSiblingParameter();
  }

  m_settingConnection.disconnect();

  if(param)
  {
    if(auto setting = getMappingSetting(param->getID().getNumber(), settings))
    {
      m_settingConnection = setting->onChange(sigc::mem_fun(this, &HardwareSourceCCLabel::onSettingsChanged));
    }
  }
}

void HardwareSourceCCLabel::onSettingsChanged(const Setting *changed)
{
  auto str = changed->getDisplayString();
  if(str == "None")
    setText("");
  else if(str == "Ch. Pressure")
    setText("Ch. Press.");
  else
    setText({ str });
}