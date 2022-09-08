#include "SelectedParameterDotSlider.h"
#include "Application.h"
#include "parameters/Parameter.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include <sigc++/sigc++.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>
#include <proxies/hwui/HWUI.h>

SelectedParameterDotSlider::SelectedParameterDotSlider(const Rect& rect)
    : super(rect)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &SelectedParameterDotSlider::setParameter)), vg);
}

SelectedParameterDotSlider::~SelectedParameterDotSlider()
{
}
