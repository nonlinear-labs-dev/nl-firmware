#include <parameters/Parameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MiniParameterDotSlider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <proxies/hwui/panel-unit/boled/setup/PedalSelectionControl.h>
#include <proxies/hwui/panel-unit/boled/setup/SetupLabel.h>
#include <proxies/hwui/Oleds.h>
#include "SmallerParameterValueLabel.h"
#include <proxies/hwui/FrameBuffer.h>

namespace detail
{
  class PedalSetupLabel : public SetupLabel
  {
   public:
    using SetupLabel::SetupLabel;

   protected:
    void setBackgroundColor(FrameBuffer &fb) const override
    {
      if(isHighlight())
      {
        fb.setColor(FrameBufferColors::C103);
      }
      else
      {
        fb.setColor(FrameBufferColors::C43);
      }
    }
  };

  class PedalSmallerParamValueLabel : public SmallerParamValueLabel
  {
   public:
    using SmallerParamValueLabel::SmallerParamValueLabel;

   protected:
    void setBackgroundColor(FrameBuffer &fb) const override
    {
      if(isHighlight())
      {
        fb.setColor(FrameBufferColors::C103);
      }
      else
      {
        fb.setColor(FrameBufferColors::C43);
      }
    }
  };
}

PedalSelectionControl::PedalSelectionControl(Parameter *param)
    : super(Rect(0, 0, 0, 0))
{
  const int h = 12;
  const int sliderHeight = 4;
  addControl(new detail::PedalSetupLabel(param->getLongName(), Rect(0, 0, 42, h)));
  addControl(new DotSlider(param, Rect(42, (h - sliderHeight) / 2, 35, sliderHeight)));
  addControl(new detail::PedalSmallerParamValueLabel(param, Rect(80, 0, 50, h)));
}

PedalSelectionControl::~PedalSelectionControl()
{
}

void PedalSelectionControl::setPosition(const Rect &rect)
{
  super::setPosition(rect);
}

void PedalSelectionControl::setBackgroundColor(FrameBuffer &fb) const
{
  auto isFocused = findControlOfType<detail::PedalSetupLabel>()->isHighlight();
  fb.setColor(isFocused ? FrameBufferColors::C103 : FrameBufferColors::C43);
}
