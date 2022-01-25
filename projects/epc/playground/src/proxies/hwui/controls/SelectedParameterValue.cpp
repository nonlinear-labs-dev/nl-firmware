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

SelectedParameterValue::SelectedParameterValue(const Rect &rect)
    : super(rect)
{
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedParameterValue::onParameterSelected)),
      getHWUI()->getCurrentVoiceGroup());

  Application::get().getHWUI()->onModifiersChanged(sigc::mem_fun(this, &SelectedParameterValue::onModifiersChanged));

  Application::get().getHWUI()->onCurrentVoiceGroupChanged(
      sigc::mem_fun(this, &SelectedParameterValue::onVoiceGroupSelectionChanged));

  Application::get().getPresetManager()->getEditBuffer()->onSoundTypeChanged(
      sigc::hide(sigc::mem_fun(this, &SelectedParameterValue::onSoundTypeChanged)), false);
}

SelectedParameterValue::~SelectedParameterValue()
{
}

void SelectedParameterValue::onModifiersChanged(ButtonModifiers mods)
{
  onParamValueChanged(
      Application::get().getPresetManager()->getEditBuffer()->getSelected(getHWUI()->getCurrentVoiceGroup()));
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
                    ->getSelected(getHWUI()->getCurrentVoiceGroup())
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
  auto selected
      = Application::get().getPresetManager()->getEditBuffer()->getSelected(getHWUI()->getCurrentVoiceGroup());
  setVisible(!selected->isDisabled());
}

PhysicalControlValueLabel::PhysicalControlValueLabel(const Rect &rect)
    : ControlWithChildren(rect)
{
  const auto &pos = rect;
  auto hW = pos.getWidth() / 2;
  auto left = Rect(0, 0, hW, pos.getHeight());
  auto right = Rect(hW, 0, hW, pos.getHeight());

  LabelStyle style { .size = FontSize::Size8,
                     .decoration = FontDecoration::Regular,
                     .justification = Font::Justification::Center,
                     .backgroundColor = FrameBufferColors::Transparent };

  m_localEnabledLabel = addControl(new LabelStyleable({ 0, 0, pos.getWidth(), pos.getHeight() }));
  m_localDisabledLabelSnd = addControl(new LabelStyleable(left));
  m_localDisabledLabelRcv = addControl(new LabelStyleable(right));
  m_localEnabledLabel->setLabelStyle(style);
  m_localDisabledLabelSnd->setLabelStyle(style);
  m_localDisabledLabelRcv->setLabelStyle(style);

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
    : Label(e)
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
  else
    setText({ str });
}

SendCCArrow::SendCCArrow(const Rect &r)
    : Control(r)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onSelectionChanged(sigc::mem_fun(this, &SendCCArrow::onParameterSelectionHappened), VoiceGroup::Global);
}

bool SendCCArrow::redraw(FrameBuffer &fb)
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C128);
  else
    fb.setColor(FrameBufferColors::C204);

  Rect r = getPosition();
  Point c = r.getCenter();

  const auto middle = r.getCenter().getY();

  fb.setPixel(r.getLeft() + 1, middle - 1);
  fb.setPixel(r.getLeft() + 2, middle - 2);
  fb.setPixel(r.getLeft() + 1, middle + 1);
  fb.setPixel(r.getLeft() + 2, middle + 2);

  for(int i = r.getLeft(); i <= r.getRight(); i += 2)
    fb.setPixel(i, middle);

  return true;
}

void SendCCArrow::onParameterSelectionHappened(const Parameter *old, const Parameter *newP)
{
  if(auto sendParameter = dynamic_cast<const HardwareSourceSendParameter *>(newP))
  {
    setVisible(sendParameter->isAssigned());
  }
  else
  {
    setVisible(false);
  }
}
