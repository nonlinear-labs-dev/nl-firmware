#include "FileInfoCaption.h"

FileInfoCaption::FileInfoCaption(const Glib::ustring &str, const Rect &pos)
    : super(pos)
{
  super::setText(str);
  super::setFontColor(FrameBuffer::Colors::C43);
}

bool FileInfoCaption::redraw(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  fb.setColor(FrameBuffer::Colors::C128);
  fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());

  super::redraw(fb);
  return true;
}