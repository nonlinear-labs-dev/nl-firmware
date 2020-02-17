#include "BasicItem.h"
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/controls/CaptionLabel.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/Oleds.h>

bool BasicItem::canEnter()
{
  return false;
}

bool BasicItem::redraw(FrameBuffer& fb)
{
  auto ret = false;
  ret |= ControlWithChildren::redraw(fb);
  ret |= drawHighlightBorder(fb);
  return ret;
}

bool BasicItem::drawHighlightBorder(FrameBuffer& fb)
{
  if(!isHighlight())
    return false;

  fb.setColor(FrameBufferColors::C179);
  fb.drawRect(getPosition());
  return true;
}

void BasicItem::init()
{
  class LeftAligned9pxCaptionLabel : public LeftAlignedLabel
  {
   public:
    using LeftAlignedLabel::LeftAlignedLabel;

    std::shared_ptr<Font> getFont() const override
    {
      return Oleds::get().getFont("Emphase_9_Regular", 9);
    }

   protected:
    void setFontColor(FrameBuffer& fb) const override
    {
      if(isHighlight())
        fb.setColor(FrameBuffer::Colors::C255);
      else
        fb.setColor(FrameBuffer::Colors::C179);
    }
  };

  auto leftHalf = getPosition();
  leftHalf.setLeft(1);
  leftHalf.setWidth((leftHalf.getWidth() / 2) - 1);

  addControl(new CaptionLabel<LeftAligned9pxCaptionLabel>(m_caption, leftHalf, true, true));
}
