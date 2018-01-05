#include "TextEditControlLetter.h"
#include <proxies/hwui/TextEditUsageMode.h>

TextEditControlLetter::TextEditControlLetter (shared_ptr<TextEditUsageMode> textUsageMode, int relativeToCursor, const Rect &pos) :
  super (pos),
  m_textUsageMode (textUsageMode),
  m_relativeToCursor (relativeToCursor)
{

}

bool TextEditControlLetter::redraw (FrameBuffer &fb)
{
  super::redraw (fb);

  if (m_relativeToCursor == 0)
  {
    const Rect &r = getPosition ();

    if (m_textUsageMode->isInserting ())
    {
      fb.drawVerticalLine(r.getX (), r.getTop(), r.getHeight());
    }
    else
    {
      fb.drawHorizontalLine(r.getX (), r.getBottom(), r.getWidth());
    }
  }
  return true;
}

Glib::ustring TextEditControlLetter::getText() const
{
  ustring ret;
  ret += m_textUsageMode->getCharAtRelativeToCursorPosition (m_relativeToCursor);
  return ret;
}
