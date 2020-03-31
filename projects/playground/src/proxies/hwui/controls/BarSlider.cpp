#include "BarSlider.h"
#include <proxies/hwui/FrameBuffer.h>
#include <groups/ParameterGroup.h>
#include <math.h>
#include <Application.h>
#include <proxies/hwui/HWUI.h>

BarSlider::BarSlider(Parameter *param, const Rect &rect)
    : super(param, rect)
{
}

BarSlider::BarSlider(const Rect &rect)
    : super(rect)
{
}

tDisplayValue BarSlider::getDrawValue() const
{
  return getValue();
}

bool BarSlider::redraw(FrameBuffer &fb)
{
  Rect r = getPosition();
  float v = getDrawValue();
  Rect smaller = r;

  if(hasBorder())
    smaller.addMargin(1, 1, 1, 1);

  if(isBiPolar())
  {
    Point center = smaller.getCenter();
    fb.setColor(FrameBufferColors::C128);

    if(drawCenterMark())
      fb.fillRect(center.getX() - 1, smaller.getTop(), 2, smaller.getHeight());

    setSliderColor(fb);

    v = std::max(std::min(v, 1.0f), -1.0f);

    if(v >= 0)
    {
      int left = center.getX();
      int width = ceil(v * smaller.getWidth() / 2);
      fb.fillRect(left, smaller.getTop(), width, smaller.getHeight());
    }
    else
    {
      int right = center.getX();
      int width = ceil(-v * smaller.getWidth() / 2);
      fb.fillRect(right - width, smaller.getTop(), width, smaller.getHeight());
    }
  }
  else
  {
    v = std::max(std::min(v, 1.0f), 0.0f);
    int width = ceil(smaller.getWidth() * v);
    setSliderColor(fb);

    if(getParameter()->enableDrawRightToLeftForVoiceGroup())
    {
      auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
      if(vg == VoiceGroup::I)
      {
        fb.fillRect(smaller.getLeft(), smaller.getTop(), width, smaller.getHeight());
      }
      else
      {
        auto realWidth = smaller.getWidth() - width;

        fb.fillRect(smaller.getRight() - realWidth, smaller.getTop(), realWidth + 1, smaller.getHeight());
      }
    }
    else
    {
      fb.fillRect(smaller.getLeft(), smaller.getTop(), width, smaller.getHeight());
    }
  }

  if(hasBorder())
  {
    r = getPosition();
    fb.setColor(FrameBufferColors::C128);
    fb.drawRect(r.getLeft(), r.getTop(), r.getWidth(), r.getHeight());
  }
  return true;
}

bool BarSlider::drawCenterMark() const
{
  return true;
}
