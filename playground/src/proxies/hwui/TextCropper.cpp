#include <proxies/hwui/controls/Control.h>
#include <utility>
#include "TextCropper.h"


Glib::ustring TextCropper::shortenStringIfNeccessary(shared_ptr<Font> font, const Glib::ustring &text, const Control &control,
                                                     int extraMargin)
{
  return preserveEndShorten(font, text, control, extraMargin);
}

ustring TextCropper::preserveEndShorten(const shared_ptr<Font> &font, const ustring &text, const Control &control, int extraMargin) {
  const auto textLen = text.length();
  const auto min = 0;
  const auto maxWidth = control.getPosition().getWidth() - extraMargin;
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

Glib::ustring TextCropper::rigidSplit(const shared_ptr<Font> &font, const ustring &text, const int maxWidth) {

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

Glib::ustring TextCropper::edgeCases(const int min, const ustring::size_type len, const ustring &ustring) {
  if(len == 0)
    return "";
  if(len < min)
    return ustring;
  if ((min + 1) == len)
    return ustring.substr(0, static_cast<ustring::size_type>(min + 1)) + "..";

  return "";
}

Glib::ustring
TextCropper::spaceBasedCropping(const ustring &text, const shared_ptr<Font> &font, double maxWidth) {
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
