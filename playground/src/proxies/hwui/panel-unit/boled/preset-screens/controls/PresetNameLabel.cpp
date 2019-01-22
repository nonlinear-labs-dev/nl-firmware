#include <proxies/hwui/TextCropper.h>
#include "PresetNameLabel.h"
#include "Application.h"
#include <presets/Preset.h>

PresetNameLabel::PresetNameLabel(const Rect &pos)
    : super(pos)
{
}

void PresetNameLabel::update(const Glib::ustring &presetName, bool selected, bool loaded)
{
  setText(presetName, selected, loaded);
}

void PresetNameLabel::drawBackground(FrameBuffer &fb)
{
  super::drawBackground(fb);

  const Rect &r = getPosition();

  if(showsLoadedPreset())
    fb.setColor(FrameBuffer::Colors::C103);
  else
    fb.setColor(FrameBuffer::Colors::C43);

  int xinset = showsSelectedPreset() ? 3 : 1;
  int yinset = showsSelectedPreset() ? 2 : 1;

  fb.fillRect(r.getX(), r.getY() + yinset, r.getWidth() - xinset, r.getHeight() - 2 * yinset);
}

Font::Justification PresetNameLabel::getJustification() const
{
  return Font::Justification::Left;
}

PresetNameLabel::StringAndSuffix PresetNameLabel::shortenStringIfNeccessary(shared_ptr<Font> font,
                                                                            const StringAndSuffix &text) const
{
  return TextCropper::shortenStringIfNeccessary(font, text.text,
                                                getPosition().getWidth() - getXOffset() - getRightMargin());
}

int PresetNameLabel::getXOffset() const
{
  return 7;
}

int PresetNameLabel::getRightMargin() const
{
  return 3;
}
