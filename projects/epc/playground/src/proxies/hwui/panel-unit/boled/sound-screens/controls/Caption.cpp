#include <proxies/hwui/panel-unit/boled/sound-screens/controls/Caption.h>
#include "proxies/hwui/panel-unit/boled/BOLED.h"
#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/FrameBuffer.h>

Caption::Caption(const Glib::ustring &text, const Rect &pos)
    : super(text, pos)
{
}

Caption::~Caption()
{
}

bool Caption::redraw(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  fb.setColor(FrameBufferColors::C128);
  fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());

  super::redraw(fb);
  return true;
}

void Caption::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C43);
}

std::shared_ptr<Font> Caption::getFont() const
{
  return Fonts::get().getFont("Emphase-8-Regular", getFontHeight());
}

int Caption::getFontHeight() const
{
  return 8;
}
