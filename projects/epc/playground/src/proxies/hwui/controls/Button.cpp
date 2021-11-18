#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/FrameBuffer.h>

Button::Button(const Glib::ustring &caption, int buttonID)
    : Button(caption, (Buttons) buttonID)
{
}

Button::Button(const Glib::ustring &caption, Buttons button)
    : super(caption, getButtonPos(button))
{
}

Button::Button(const Glib::ustring &caption, const Rect &rect)
    : super(caption, rect)
{
}

Button::~Button()
{
}

void Button::blind(bool b)
{
  if(m_blind != b)
  {
    m_blind = b;
    setDirty();
  }
}

void Button::setFontColor(FrameBuffer &fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C255);
  else
    fb.setColor(FrameBufferColors::C204);
}

FrameBufferColors Button::getBackgroundColor() const
{
  if(!m_enabled)
    return FrameBufferColors::C43;
  else if(isHighlight())
    return FrameBufferColors::C128;
  else
    return FrameBufferColors::C77;
}

FrameBufferColors Button::getForegroundColor() const
{
  if(!m_enabled)
    return FrameBufferColors::C77;
  else if(isHighlight())
    return FrameBufferColors::C179;
  else
    return FrameBufferColors::C103;
}

bool Button::redraw(FrameBuffer &fb)
{
  auto r = getPosition();

  fb.setColor(getBackgroundColor());
  fb.fillRect(r.getMargined(2, 2));

  fb.setColor(getForegroundColor());

  fb.drawHorizontalLine(r.getLeft() + 1, r.getTop(), r.getWidth() - 2);
  fb.drawVerticalLine(r.getLeft(), r.getTop() + 1, r.getHeight() - 2);
  fb.drawHorizontalLine(r.getLeft() + 1, r.getBottom(), r.getWidth() - 2);
  fb.drawVerticalLine(r.getRight(), r.getY() + 1, r.getHeight() - 2);

  if(!m_blind)
    super::redraw(fb);

  return true;
}

void Button::setEnable(bool e)
{
  if(isEnabled() != e)
  {
    m_enabled = e;
    setDirty();
  }
}

bool Button::isEnabled() const
{
  return m_enabled;
}

Rect Button::getButtonPos(Buttons n)
{
  switch(n)
  {
    default:
    case Buttons::BUTTON_A:
      return Rect(3, 51, 58, 11);

    case Buttons::BUTTON_B:
      return Rect(67, 51, 58, 11);

    case Buttons::BUTTON_C:
      return Rect(131, 51, 58, 11);

    case Buttons::BUTTON_D:
      return Rect(195, 51, 58, 11);
  }
}
