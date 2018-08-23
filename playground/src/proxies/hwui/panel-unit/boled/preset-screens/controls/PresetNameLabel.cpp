#include "PresetNameLabel.h"
#include "Application.h"
#include "presets/Preset.h"

PresetNameLabel::PresetNameLabel (const Rect &pos) :
    super (pos)
{
}

PresetNameLabel::~PresetNameLabel ()
{
}

void PresetNameLabel::update(const Glib::ustring &presetName, bool selected, bool loaded)
{
  setText(presetName, selected, loaded);
}

void PresetNameLabel::drawBackground (FrameBuffer &fb)
{
  super::drawBackground (fb);

  const Rect &r = getPosition ();

  if (showsLoadedPreset())
    fb.setColor (FrameBuffer::Colors::C103);
  else
    fb.setColor (FrameBuffer::Colors::C43);

  int xinset = showsSelectedPreset() ? 3 : 1;
  int yinset = showsSelectedPreset() ? 2 : 1;

  fb.fillRect (r.getX (), r.getY () + yinset, r.getWidth () - xinset, r.getHeight () - 2 * yinset);
}

Font::Justification PresetNameLabel::getJustification () const
{
  return Font::Justification::Left;
}

Glib::ustring PresetNameLabel::shortenStringIfNeccessary (shared_ptr<Font> font, const Glib::ustring &text) const
{
  return preserveEndShorten(font, text);
}

Glib::ustring PresetNameLabel::shortenStringOld(shared_ptr<Font> font, Glib::ustring& text) const {

  auto width = getPosition().getWidth() - getXOffset() - getRightMargin();
  auto c = text;

  for(size_t len = text.length(); len > 0 && (font->getStringWidth(c) >= width); len--)
  {
    c = text.substr(0, len) + "..";
  }

  return c;
}

Glib::ustring PresetNameLabel::preserveEndShorten(shared_ptr<Font> font, const Glib::ustring &text) const {
  auto maxWidth = getPosition().getWidth() - getXOffset() - getRightMargin();
  auto c = text;
  if(maxWidth == 0)
    return "";
  auto roughTwoThirds = (text.length() / 3) * 2;
  if(auto pos = text.find_last_of(' ')) {
    if(pos < text.length() && pos >= roughTwoThirds) {
      auto partAfterSpace = c.substr(pos + 1, c.length() - 1);
      Glib::ustring front = "";
      for(int i = 0;
          (font->getStringWidth(to_string(front + partAfterSpace + "...")) < maxWidth);
          i++)
      {
        front += text[i];
      }
      return front + ".." + partAfterSpace;
    }
  }
  return shortenStringOld(font, c);
}

int PresetNameLabel::getXOffset () const
{
  return 7;
}

int PresetNameLabel::getRightMargin() const
{
  return 3;
}
