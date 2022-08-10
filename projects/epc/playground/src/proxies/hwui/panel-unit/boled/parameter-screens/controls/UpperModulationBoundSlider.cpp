#include <Application.h>
#include <parameters/ModulateableParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UpperModulationBoundSlider.h>
#include <proxies/hwui/HWUI.h>

UpperModulationBoundSlider::UpperModulationBoundSlider(const Rect &pos)
    : super(pos)
{
}

UpperModulationBoundSlider::~UpperModulationBoundSlider()
{
}

UpperModulationBoundSlider::BoundPosition UpperModulationBoundSlider::getBoundPosition() const
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  if(auto p
     = dynamic_cast<ModulateableParameter *>(Application::get().getPresetManager()->getEditBuffer()->getSelected(vg)))
    return p->getModulationAmount() >= 0 ? BoundPosition::Right : BoundPosition::Left;

  return BoundPosition::Right;
}
