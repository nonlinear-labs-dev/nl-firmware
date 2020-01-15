#include <glibmm/ustring.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/Oleds.h>
#include "FileNameHeadlineLabel.h"
#include <proxies/hwui/FrameBuffer.h>

FileNameHeadlineLabel::FileNameHeadlineLabel(const Glib::ustring &str, const Rect &pos)
    : super(pos)
{
  super::setText(str);
  super::setFontColor(FrameBufferColors::C179);
}

std::shared_ptr<Font> FileNameHeadlineLabel::getFont() const
{
  return Oleds::get().getFont("Emphase_9_Bold", getFontHeight());
}

bool FileNameHeadlineLabel::redraw(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  fb.setColor(FrameBufferColors::C77);
  fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());

  super::redraw(fb);
  return true;
}