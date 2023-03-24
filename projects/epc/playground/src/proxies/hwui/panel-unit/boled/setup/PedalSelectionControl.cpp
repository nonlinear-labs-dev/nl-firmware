#include <parameters/Parameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MiniParameterDotSlider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <proxies/hwui/panel-unit/boled/setup/PedalSelectionControl.h>
#include <proxies/hwui/panel-unit/boled/setup/SetupLabel.h>
#include <proxies/hwui/Oleds.h>
#include "SmallerParameterValueLabel.h"
#include "SetupLayout.h"
#include "PedalEditor.h"
#include "parameters/PedalParameter.h"
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>

namespace detail
{
  static bool isInPedalEditor()
  {
    auto hwui = Application::get().getHWUI();

    if(auto setupLayout = dynamic_cast<SetupLayout *>(hwui->getPanelUnit().getEditPanel().getBoled().getLayout().get()))
    {
      return setupLayout->findControlOfType<PedalEditor>() != nullptr;
    }

    return false;
  }

  class PedalSetupLabel : public SetupLabel
  {
   public:
    using SetupLabel::SetupLabel;

   protected:
    void setBackgroundColor(FrameBuffer &fb) const override
    {
      if(isHighlight() && !isInPedalEditor())
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
      if(isHighlight() && !isInPedalEditor())
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

PedalSelectionControl::PedalSelectionControl(PedalParameter *param)
    : super(Rect(0, 0, 0, 0))
{
  const int h = 12;
  const int sliderHeight = 4;
  addControl(new detail::PedalSetupLabel(param->getLongNameWithoutSuffix(), Rect(0, 0, 42, h)));
  addControl(new DotSlider(param, Rect(42, (h - sliderHeight) / 2, 35, sliderHeight)));
  addControl(new detail::PedalSmallerParamValueLabel(param, Rect(80, 0, 50, h)));
}

PedalSelectionControl::~PedalSelectionControl() = default;

void PedalSelectionControl::setPosition(const Rect &rect)
{
  super::setPosition(rect);
}

void PedalSelectionControl::setBackgroundColor(FrameBuffer &fb) const
{
  auto isFocused = findControlOfType<detail::PedalSetupLabel>()->isHighlight() && !detail::isInPedalEditor();
  fb.setColor(isFocused ? FrameBufferColors::C103 : FrameBufferColors::C43);
}
