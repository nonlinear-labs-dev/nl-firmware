#include <proxies/hwui/Oleds.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/BankNameHeadlineLabel.h>
#include <proxies/hwui/FrameBuffer.h>

BankNameHeadlineLabel::BankNameHeadlineLabel(const Rect &pos)
    : super(pos)
{
  setText("Bank");
}

std::shared_ptr<Font> BankNameHeadlineLabel::getFont() const
{
  return Fonts::get().getFont("Emphase-9-Bold", getFontHeight());
}

int BankNameHeadlineLabel::getFontHeight() const
{
  return 9;
}

void BankNameHeadlineLabel::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C255);
}

bool BankNameHeadlineLabel::redraw(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  fb.setColor(FrameBufferColors::C77);
  fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());

  super::redraw(fb);
  return true;
}
