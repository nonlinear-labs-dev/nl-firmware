#include "PresetTypeLabel.h"
#include <proxies/hwui/FrameBuffer.h>

PresetTypeLabel::PresetTypeLabel(const Rect &pos)
    : PresetLabel(pos)
{
}

void PresetTypeLabel::update(const SoundType &type, bool selected, bool loaded)
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

void PresetTypeLabel::drawBackground(FrameBuffer &fb)
{
  PresetLabel::drawBackground(fb);

  const Rect &r = getPosition();

  if(showsLoadedPreset())
    fb.setColor(FrameBufferColors::C103);
  else
    fb.setColor(FrameBufferColors::C43);

  int xinset = showsSelectedPreset() ? 3 : 1;
  int yinset = showsSelectedPreset() ? 2 : 1;

  fb.fillRect(r.getX(), r.getY() + yinset, r.getWidth() - xinset, r.getHeight() - 2 * yinset);
}

std::string PresetTypeLabel::typeToString(const SoundType &type)
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
