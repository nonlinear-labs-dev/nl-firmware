#include "CaptionLabel.h"
#include <proxies/hwui/FrameBuffer.h>

CaptionLabelBase::CaptionLabelBase(bool changeHighlight, bool changeBackground)
    : m_changeHighlight(changeHighlight)
    , m_changeHighlightBackground(changeBackground)
{
}

CaptionLabelBase::~CaptionLabelBase()
{
}

int CaptionLabelBase::getXOffset() const
{
  return 2;
}

void CaptionLabelBase::setFontColor(FrameBuffer &fb) const
{
  if(m_changeHighlight)
  {
    if(dynamic_cast<const Control *>(this)->isHighlight())
      fb.setColor(FrameBufferColors::C255);
    else
      fb.setColor(FrameBufferColors::C128);
  }
  else
  {
    fb.setColor(FrameBufferColors::C128);
  }
}

void CaptionLabelBase::setBackgroundColor(FrameBuffer &fb) const
{
  if(m_changeHighlightBackground)
  {
    if(dynamic_cast<const Control *>(this)->isHighlight())
      fb.setColor(FrameBufferColors::C103);
    else
      fb.setColor(FrameBufferColors::C43);
  }
  else
  {
    fb.setColor(FrameBufferColors::C43);
  }
}
