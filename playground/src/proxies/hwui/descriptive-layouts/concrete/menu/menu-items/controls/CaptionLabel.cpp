#include "CaptionLabel.h"
#include <proxies/hwui/FrameBuffer.h>

CaptionLabelBase::CaptionLabelBase(bool changeHighlight)
    : m_changeHighlight(changeHighlight)
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
      fb.setColor(FrameBufferColors::C179);
  }
  else
  {
    fb.setColor(FrameBufferColors::C179);
  }
}

void CaptionLabelBase::setBackgroundColor(FrameBuffer &fb) const
{
  if(m_changeHighlight)
  {
    if(dynamic_cast<const Control *>(this)->isHighlight())
      fb.setColor(FrameBufferColors::C128);
    else
      fb.setColor(FrameBufferColors::C43);
  }
  else
  {
    fb.setColor(FrameBufferColors::C43);
  }
}
