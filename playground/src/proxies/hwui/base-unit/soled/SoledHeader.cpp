#include <proxies/hwui/base-unit/soled/SoledHeader.h>
#include <proxies/hwui/FrameBuffer.h>

SoledHeader::SoledHeader(const Glib::ustring &text, const Rect &pos)
    : base_type(text, pos)
{
}

SoledHeader::~SoledHeader()
{
}

void SoledHeader::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C43);
}

bool SoledHeader::redraw(FrameBuffer &fb)
{
  auto pos = getPosition();
  fb.setColor(FrameBufferColors::C255);
  fb.fillRect(pos);
  Label::redraw(fb);
  return true;
}

int SoledHeader::getXOffset() const
{
  return 1;
}
