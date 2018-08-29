#include "PresetNameLabel.h"
#include "Application.h"
#include "presets/Preset.h"

PresetNameLabel::PresetNameLabel (const Rect &pos) :
    super (pos)
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

Glib::ustring PresetNameLabel::preserveEndShorten(shared_ptr<Font> font, const Glib::ustring &text) const {
  const auto textLen = text.length();
  const auto min = 0;
  const auto maxWidth = getPosition().getWidth() - getXOffset() - getRightMargin();
  auto edgeCaseString = edgeCases(min, textLen, text);
  if(!edgeCaseString.empty()) {
    return edgeCaseString;
  }

  auto spaceSplitString = spaceBasedCropping(text, font, maxWidth * 0.95);
  if(!spaceSplitString.empty()) {
    return spaceSplitString;
  }

  return rigidSplit(font,text,maxWidth);
}

Glib::ustring PresetNameLabel::rigidSplit(const shared_ptr<Font> &font, const ustring &text, const int maxWidth) const {

  ustring front;
  ustring back;
  const ustring padding = "..";
  int i = 0;
  while(font->getStringWidth(front + padding) < maxWidth / 2) {
    front.push_back(text.at(static_cast<ustring::size_type>(i++)));
  }
  i = static_cast<int>(text.length() - 1);
  while(font->getStringWidth(back + padding) < maxWidth / 2) {
    back.insert(back.begin(), text.at(static_cast<ustring::size_type>(i--)));
  }
  return front + ".." + back;
}

int PresetNameLabel::getXOffset () const
{
  return 7;
}

int PresetNameLabel::getRightMargin() const
{
  return 3;
}

Glib::ustring PresetNameLabel::edgeCases(const int min, const ustring::size_type len, const ustring &ustring) const {
  if(len == 0)
    return "";
  if(len < min)
    return ustring;
  if ((min + 1) == len)
    return ustring.substr(0, static_cast<ustring::size_type>(min + 1)) + "..";

  return "";
}

Glib::ustring
PresetNameLabel::spaceBasedCropping(const ustring &text, shared_ptr<Font> font, double maxWidth) const {
  const auto &c = text;
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
  return "";
}

