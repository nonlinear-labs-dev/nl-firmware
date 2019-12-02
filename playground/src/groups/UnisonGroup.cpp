#include "UnisonGroup.h"

#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"

#include "parameters/scale-converters/Linear12CountScaleConverter.h"
#include "parameters/scale-converters/Linear360DegreeScaleConverter.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "parameters/scale-converters/Fine12STScaleConverter.h"
#include "parameters/scale-converters/FineBipolar12STScaleConverter.h"
#include <parameters/ModulateableParameterWithUnusualModUnit.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/UnmodulatebaleParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/HWUI.h>

UnisonGroup::UnisonGroup(ParameterDualGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Unison", vg }, "Unison", "Unison", "Unison")
{
}

UnisonGroup::~UnisonGroup() = default;

namespace Detail
{
  class UnisonLayout : public UnmodulateableParameterSelectLayout2
  {
   protected:
    void init() override
    {
      UnmodulateableParameterSelectLayout2::init();

      for(auto &c : getControls<Button>())
      {
        if(c->getButtonPos(Buttons::BUTTON_C) == c->getPosition())
        {
          c->setText({ "^", 0 });
        }
      }
    }

    bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override
    {
      if(down && i == Buttons::BUTTON_C)
      {
        Application::get().getHWUI()->setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Voices });
        return true;
      }

      return ParameterSelectLayout2::onButton(i, down, modifiers);
    }
  };

  template <typename tParameter> class UnisonParameter : public tParameter
  {
   public:
    template <typename... tArgs>
    explicit UnisonParameter(tArgs... args)
        : tParameter(args...){};

    DFBLayout *createLayout(FocusAndMode focusAndMode) const override
    {
      switch(focusAndMode.mode)
      {
        case UIMode::Select:
        default:
          return new UnisonLayout();
        case UIMode::Edit:
          return new UnmodulateableParameterEditLayout2();
        case UIMode::Info:
          return new ParameterInfoLayout();
      }
    }
  };
}

void UnisonGroup::init()
{
  appendParameter(new Detail::UnisonParameter<Parameter>(
      this, ParameterId{ 249, getVoiceGroup() }, ScaleConverter::get<Linear12CountScaleConverter>(), 0, 11, 11));

  appendParameter(new Detail::UnisonParameter<ModulateableParameterWithUnusualModUnit>(
      this, ParameterId{ 250, getVoiceGroup() }, ScaleConverter::get<Fine12STScaleConverter>(),
      ScaleConverter::get<FineBipolar12STScaleConverter>(), 0, 120, 12000));

  appendParameter(new Detail::UnisonParameter<Parameter>(
      this, ParameterId{ 252, getVoiceGroup() }, ScaleConverter::get<Linear360DegreeScaleConverter>(), 0, 360, 3600));

  appendParameter(new Detail::UnisonParameter<Parameter>(
      this, ParameterId{ 253, getVoiceGroup() }, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));
}
