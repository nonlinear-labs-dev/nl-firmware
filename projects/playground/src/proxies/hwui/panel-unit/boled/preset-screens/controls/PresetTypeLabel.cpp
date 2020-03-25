#include "PresetTypeLabel.h"
#include <proxies/hwui/FrameBuffer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>
#include <proxies/hwui/HWUI.h>
#include <memory>
#include <proxies/hwui/HWUIHelper.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>

PresetTypeLabel::PresetTypeLabel(const Rect &pos)
    : Control(pos)
{
  m_editbufferConnection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &PresetTypeLabel::onEditBufferChanged));

  m_voiceGroupChanged = Application::get().getHWUI()->onCurrentVoiceGroupChanged(
      sigc::mem_fun(this, &PresetTypeLabel::onVoiceGroupChanged));
}

PresetTypeLabel::~PresetTypeLabel()
{
  m_editbufferConnection.disconnect();
  m_voiceGroupChanged.disconnect();
}

void PresetTypeLabel::onEditBufferChanged()
{
  if(selectedPreset)
    update(selectedPreset);
}

void PresetTypeLabel::onVoiceGroupChanged(const VoiceGroup &vg)
{
  if(selectedPreset)
    update(selectedPreset);
}

bool PresetTypeLabel::redraw(FrameBuffer &fb)
{
  if(m_currentControl)
    return m_currentControl->redraw(fb);
  return false;
}

void SinglePresetTypeLabel::drawBackground(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  if(showsLoadedPreset())
  {
    fb.setColor(FrameBufferColors::C103);
    int yinset = showsSelectedPreset() ? 2 : 1;
    int xinset = 2;
    fb.fillRect(r.getX(), r.getY() + yinset, r.getWidth() - xinset, r.getHeight() - 2 * yinset);
  }
}

void PresetTypeLabel::update(const Preset *newSelection)
{
  selectedPreset = newSelection;
  auto isDualEditBuffer = Application::get().getPresetManager()->getEditBuffer()->isDual();

  if(HWUIHelper::isLoadToPartActive() && isDualEditBuffer)
  {
    m_currentControl = std::make_unique<DualPresetTypeLabel>(getPosition());
    auto dualLabel = dynamic_cast<DualPresetTypeLabel *>(m_currentControl.get());
    dualLabel->update(newSelection);
  }
  else
  {
    m_currentControl = std::make_unique<SinglePresetTypeLabel>(getPosition());
    auto singleLabel = dynamic_cast<SinglePresetTypeLabel *>(m_currentControl.get());
    singleLabel->update(newSelection);
  }
}

std::string typeToString(const SoundType &type)
{
  switch(type)
  {
    case SoundType::Single:
    case SoundType::Invalid:
      return "";
    case SoundType::Layer:
      return "\uE0F3";
    case SoundType::Split:
      return "\uE0F0";
  }
  return {};
}

SinglePresetTypeLabel::SinglePresetTypeLabel(const Rect &r)
    : PresetLabel(r)
{
}

void SinglePresetTypeLabel::update(const Preset *newPreset)
{
  if(newPreset)
  {
    auto type = newPreset->getType();
    auto loaded
        = Application::get().getPresetManager()->getEditBuffer()->getUUIDOfLastLoadedPreset() == newPreset->getUuid();

    setText(typeToString(type), true, loaded);
  }
}

DualPresetTypeLabel::DualPresetTypeLabel(const Rect &r)
    : Control(r)
{
}

bool DualPresetTypeLabel::redraw(FrameBuffer &fb)
{
  if(!hasPreset)
  {
    fb.setColor(FrameBufferColors::Transparent);
    fb.fillRect(getPosition());
    return true;
  }
  else
  {
    switch(m_presetType)
    {
      case SoundType::Single:
        return drawSingle(fb);
      case SoundType::Layer:
        return drawLayer(fb);
      case SoundType::Split:
        return drawSplit(fb);
      case SoundType::Invalid:
        return false;
    }
  }
}

bool DualPresetTypeLabel::drawLayer(FrameBuffer &buffer)
{
  buffer.setColor(m_inidicateI ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX(), getPosition().getY() + 2, 11, 5);
  if(m_selectedI)
  {
    buffer.setColor(FrameBufferColors::C179);
    buffer.drawRect(getPosition().getX(), getPosition().getY() + 2, 11, 5);
  }

  buffer.setColor(m_inidicateII ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX(), getPosition().getY() + 9, 11, 5);
  if(m_selectedII)
  {
    buffer.setColor(FrameBufferColors::C179);
    buffer.drawRect(getPosition().getX(), getPosition().getY() + 9, 11, 5);
  }
  return true;
}

bool DualPresetTypeLabel::drawSplit(FrameBuffer &buffer)
{

  buffer.setColor(m_inidicateI ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX(), getPosition().getY() + 2, 5, 12);
  if(m_selectedI)
  {
    buffer.setColor(FrameBufferColors::C179);
    buffer.drawRect(getPosition().getX(), getPosition().getY() + 2, 5, 12);
  }

  buffer.setColor(m_inidicateII ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX() + 6, getPosition().getY() + 2, 5, 12);
  if(m_selectedII)
  {
    buffer.setColor(FrameBufferColors::C179);
    buffer.drawRect(getPosition().getX() + 6, getPosition().getY() + 2, 5, 12);
  }
  return true;
}

bool DualPresetTypeLabel::drawSingle(FrameBuffer &buffer)
{
  buffer.setColor(m_inidicateI ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX(), getPosition().getY() + 2, 12, 12);
  if(m_selectedI)
  {
    buffer.setColor(FrameBufferColors::C179);
    buffer.drawRect(getPosition().getX(), getPosition().getY() + 2, 12, 12);
  }

  return true;
}

void DualPresetTypeLabel::update(const Preset *selected)
{
  hasPreset = selected != nullptr;

  if(selected)
  {
    auto currentVGFocus = Application::get().getHWUI()->getCurrentVoiceGroup();
    const auto origin = Application::get().getPresetManager()->getEditBuffer()->getPartOrigin(currentVGFocus);

    auto currentLayout = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().getLayout().get();
    auto presetManagerLayout = dynamic_cast<PresetManagerLayout *>(currentLayout);

    auto selection = presetManagerLayout->getPresetPartSelection(currentVGFocus);

    const auto &presetUUID = selected->getUuid();

    m_presetType = selected->getType();

    m_anySelected = selection->m_preset->getUuid() == selected->getUuid();
    m_selectedI = m_anySelected && selection->m_voiceGroup == VoiceGroup::I;
    m_selectedII = m_anySelected && selection->m_voiceGroup == VoiceGroup::II;

    m_presetLoaded = origin.presetUUID == presetUUID;
    m_inidicateI = m_presetLoaded && origin.sourceGroup == VoiceGroup::I;
    m_inidicateII = m_presetLoaded && origin.sourceGroup == VoiceGroup::II;

    setDirty();
  }
}
