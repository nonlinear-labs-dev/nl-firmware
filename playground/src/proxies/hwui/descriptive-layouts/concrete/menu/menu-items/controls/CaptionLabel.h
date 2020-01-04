#pragma once
#include "proxies/hwui/controls/Rect.h"
#include "proxies/hwui/controls/Control.h"

class CaptionLabelBase
{
 public:
  CaptionLabelBase(bool changeHighlight);
  virtual ~CaptionLabelBase();

 protected:
  int getXOffset() const;
  void setFontColor(FrameBuffer& fb) const;
  void setBackgroundColor(FrameBuffer& fb) const;

  bool m_changeHighlight;
};

template <typename tLabelType> class CaptionLabel : public tLabelType, public CaptionLabelBase
{
 public:
  CaptionLabel(const Glib::ustring& caption, const Rect& rect, bool changeHighlight)
      : tLabelType(caption, rect)
      , CaptionLabelBase(changeHighlight)
  {
  }

 protected:
  int getXOffset() const override
  {
    return CaptionLabelBase::getXOffset();
  }

  void setFontColor(FrameBuffer& fb) const override
  {
    CaptionLabelBase::setFontColor(fb);
  }

  void setBackgroundColor(FrameBuffer& fb) const override
  {
    CaptionLabelBase::setBackgroundColor(fb);
  }
};
