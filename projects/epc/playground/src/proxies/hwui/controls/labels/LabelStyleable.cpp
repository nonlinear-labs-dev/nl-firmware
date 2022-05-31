#include "LabelStyleable.h"
#include <proxies/hwui/FrameBuffer.h>

LabelStyleable::LabelStyleable(const Rect& pos)
    : Label { pos }
{
}

LabelStyleable::LabelStyleable(const StringAndSuffix& text, const Rect& pos)
    : Label { pos }
{
}

LabelStyleable::LabelStyleable(const StringAndSuffix& text, const Rect& pos, LabelStyle style)
    : Label { text, pos }
    , m_styleSheet { style }
{
  setLabelStyle(m_styleSheet);
}

LabelStyleable::LabelStyleable(const Rect& pos, LabelStyle style)
    : Label { pos }
    , m_styleSheet { style }
{
  setLabelStyle(m_styleSheet);
}

void LabelStyleable::setLabelStyle(LabelStyle style)
{
  if(style != m_styleSheet)
  {
    m_styleSheet = style;
    setDirty();
  }
}

Font::Justification LabelStyleable::getJustification() const
{
  return m_styleSheet.justification;
}

void LabelStyleable::setBackgroundColor(FrameBuffer& fb) const
{
  fb.setColor(m_styleSheet.backgroundColor);
}

std::shared_ptr<Font> LabelStyleable::getFont() const
{
  try
  {
    std::stringstream fontStream {};

    fontStream << "Emphase-";

    auto fontHeight = static_cast<int>(m_styleSheet.size);
    if(fontHeight < 8) //FIXME either remove size7 or request Emphase-7-Style.ttf s
      fontStream << 8;
    else
      fontStream << fontHeight;

    switch(m_styleSheet.decoration)
    {
      case FontDecoration::Regular:
        fontStream << "-Regular";
        break;
      case FontDecoration::Bold:
        fontStream << "-Bold";
        break;
    }

    auto rawStr = fontStream.str();
    return Fonts::get().getFont(rawStr, fontHeight);
  }
  catch(...)
  {
    ExceptionTools::errorLogCurrentException();
  }

  return Fonts::get().getFont("Emphase-8-Regular", getFontHeight());
}

LabelStyle LabelStyleable::getCurrentStyle() const
{
  return m_styleSheet;
}
