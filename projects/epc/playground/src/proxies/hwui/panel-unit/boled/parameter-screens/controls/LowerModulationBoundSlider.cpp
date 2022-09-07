#include <Application.h>
#include <parameters/ModulateableParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/LowerModulationBoundSlider.h>
#include <proxies/hwui/HWUI.h>

LowerModulationBoundSlider::LowerModulationBoundSlider(const Rect &pos)
    : super(pos)
{
}

LowerModulationBoundSlider::~LowerModulationBoundSlider()
{
}

void LowerModulationBoundSlider::drawBackground(FrameBuffer &fb)
{
}

LowerModulationBoundSlider::BoundPosition LowerModulationBoundSlider::getBoundPosition() const
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(auto p = dynamic_cast<ModulateableParameter *>(eb->getSelected(vg)))
    return p->getModulationAmount() < 0 ? BoundPosition::Right : BoundPosition::Left;

  return BoundPosition::Left;
}
