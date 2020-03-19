#include "PresetTypeLabel.h"
#include <proxies/hwui/FrameBuffer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/LoadToPartSetting.h>
#include <device-settings/DirectLoadSetting.h>
#include <proxies/hwui/HWUI.h>

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

  if(isDirectLoad && isLoadToPart && isDualEditBuffer)
  {
    m_currentControl.reset(new DualPresetTypeLabel(getPosition()));
    auto dualLabel = dynamic_cast<DualPresetTypeLabel *>(m_currentControl.get());
    dualLabel->
  }
  else
  {
    m_currentControl.reset(new SinglePresetTypeLabel(getPosition()));
    auto singleLabel = dynamic_cast<SinglePresetTypeLabel *>(m_currentControl.get());
    singleLabel->update(type, selected, loaded);
  }

  m_currentControl->
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

void SinglePresetTypeLabel::update(const SoundType &type, bool selected, bool loaded)
{
  if(type != SoundType::Invalid)
  {
    setText(typeToString(type), selected, loaded);
  }
  else
  {
    setText("", selected, loaded);
  }
}

DualPresetTypeLabel::DualPresetTypeLabel(const Rect &r)
    : Control(r)
{
}

bool DualPresetTypeLabel::redraw(FrameBuffer &fb)
{
  auto type = Application::get().getPresetManager()->getEditBuffer()->getType();

  switch(type)
  {
    case SoundType::Single:
    case SoundType::Invalid:
      return false;
    case SoundType::Layer:
      return drawLayer(fb);
    case SoundType::Split:
      return drawSplit(fb);
  }
}

bool DualPresetTypeLabel::drawLayer(FrameBuffer &buffer)
{
  auto loadIntoVG = Application::get().getHWUI()->getCurrentVoiceGroup();
  auto pm = Application::get().getPresetManager();
  auto eb = pm->getEditBuffer();
  auto loadedPresetUUID = eb->getUUIDOfLastLoadedPreset();

  return false;
}

bool DualPresetTypeLabel::drawSplit(FrameBuffer &buffer)
{
  return false;
}

void DualPresetTypeLabel::update(const Preset *selected)
{
  if(selected)
  {
    
  }
}
