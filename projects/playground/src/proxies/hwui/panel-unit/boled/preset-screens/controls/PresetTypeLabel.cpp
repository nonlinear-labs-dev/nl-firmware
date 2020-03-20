#include "PresetTypeLabel.h"
#include <proxies/hwui/FrameBuffer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/LoadToPartSetting.h>
#include <device-settings/DirectLoadSetting.h>
#include <proxies/hwui/HWUI.h>
#include <memory>

PresetTypeLabel::PresetTypeLabel(const Rect &pos)
    : Control(pos)
{
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
  auto isDirectLoad = Application::get().getSettings()->getSetting<DirectLoadSetting>()->get();
  auto isLoadToPart = Application::get().getSettings()->getSetting<LoadToPartSetting>()->get();
  auto isDualEditBuffer = Application::get().getPresetManager()->getEditBuffer()->getType() != SoundType::Single;

  if(isLoadToPart && isDualEditBuffer)
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
  switch(m_presetType)
  {
    case SoundType::Single:
    case SoundType::Invalid:
      return false;
    case SoundType::Layer:
      return drawLayer(fb);
    case SoundType::Split:
      return drawSplit(fb);
  }

  return false;
}

bool DualPresetTypeLabel::drawLayer(FrameBuffer &buffer)
{
  buffer.setColor(m_inidicateI ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX(), getPosition().getY() + 2, 11, 5);
  buffer.setColor(m_inidicateII ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX(), getPosition().getY() + 9, 11, 5);

  return true;
}

bool DualPresetTypeLabel::drawSplit(FrameBuffer &buffer)
{
  buffer.setColor(m_inidicateI ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX(), getPosition().getY() + 2, 5, 12);
  buffer.setColor(m_inidicateII ? FrameBufferColors::C255 : FrameBufferColors::C128);
  buffer.fillRect(getPosition().getX() + 6, getPosition().getY() + 2, 5, 12);

  return true;
}

void DualPresetTypeLabel::update(const Preset *selected)
{
  if(selected)
  {
    auto currentVGFocus = Application::get().getHWUI()->getCurrentVoiceGroup();
    const auto origin = Application::get().getPresetManager()->getEditBuffer()->getPartOrigin(currentVGFocus);

    const auto &presetUUID = selected->getUuid();

    m_presetType = selected->getType();

    m_anyLoaded = origin.presetUUID == presetUUID;
    m_inidicateI = origin.sourceGroup == VoiceGroup::I;
    m_inidicateII = origin.sourceGroup == VoiceGroup::II;

    setDirty();
  }
}
