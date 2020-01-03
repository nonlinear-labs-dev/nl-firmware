#pragma once
#include "proxies/hwui/controls/Rect.h"
#include "proxies/hwui/controls/Control.h"
#include <proxies/hwui/FrameBuffer.h>

template <typename tLabelType> class CaptionLabel : public tLabelType
{
 public:
  CaptionLabel(const Glib::ustring& caption, const Rect& rect, bool changeHighlight)
      : tLabelType(caption, rect)
      , m_changeHighlight(changeHighlight)
  {
  }

 protected:
  int getXOffset() const override
  {
    return 2;
  }

  void setFontColor(FrameBuffer& fb) const override
  {
    if(m_changeHighlight)
    {
      if(tLabelType::isHighlight())
        fb.setColor(FrameBuffer::C255);
      else
        fb.setColor(FrameBuffer::C179);
    }
    else
    {
      fb.setColor(FrameBuffer::C179);
    }
  }

  void setBackgroundColor(FrameBuffer& fb) const override
  {
    if(m_changeHighlight)
    {
      if(tLabelType::isHighlight())
        fb.setColor(FrameBuffer::C128);
      else
        fb.setColor(FrameBuffer::C43);
    }
    else
    {
      fb.setColor(FrameBuffer::C43);
    }
  }

  bool m_changeHighlight;
};
