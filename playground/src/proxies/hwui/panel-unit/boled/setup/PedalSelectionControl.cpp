#include <parameters/Parameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MiniParameterDotSlider.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <proxies/hwui/panel-unit/boled/setup/PedalSelectionControl.h>
#include <proxies/hwui/panel-unit/boled/setup/SetupLabel.h>
#include <proxies/hwui/Oleds.h>

class SmallerParamValueLabel : public ParameterValueLabel
{
  public:
    using ParameterValueLabel::ParameterValueLabel;

    shared_ptr<Font> getFont () const
    {
      return Oleds::get ().getFont ("Emphase_8_Regular", getFontHeight ());
    }

    int getFontHeight () const
    {
      return 8;
    }
};

PedalSelectionControl::PedalSelectionControl (Parameter *param) :
    super (Rect (0, 0, 0, 0))
{
  const int h = 12;
  const int sliderHeight = 2;
  addControl (new SetupLabel (param->getLongName (), Rect (0, 0, 43, h)));
  addControl (new MiniParameterDotSlider (param, Rect (43, (h - sliderHeight) / 2, 42, sliderHeight)));
  addControl (new SmallerParamValueLabel (param, Rect (85, 0, 42, h)));
}

PedalSelectionControl::~PedalSelectionControl ()
{
}

void PedalSelectionControl::setPosition (const Rect &rect)
{
  super::setPosition (rect);
}
