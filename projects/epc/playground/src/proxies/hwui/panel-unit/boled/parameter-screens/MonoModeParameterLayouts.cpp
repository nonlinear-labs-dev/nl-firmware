#include "MonoModeParameterLayouts.h"
#include "use-cases/SettingsUseCases.h"
#include <Application.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MonoParameterCarousel.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/HWUI.h>
#include <parameters/mono-mode-parameters/ModulateableMonoParameter.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

Parameter *MonoModeParameterLayout::getCurrentParameter() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  return eb->getSelected(vg);
}

MonoModeParameterLayout::MonoModeParameterLayout()
    : UnmodulateableParameterSelectLayout2()
{
}

void MonoModeParameterLayout::init()
{
  UnmodulateableParameterSelectLayout2::init();

  for(auto &c : getControls<Button>())
  {
    if(c->getButtonPos(Buttons::BUTTON_C) == c->getPosition())
    {
      c->setText(StringAndSuffix { "back..", 0 });
    }
  }
}

Carousel *MonoModeParameterLayout::createCarousel(const Rect &rect)
{
  return new MonoParameterCarousel(rect);
}

bool MonoModeParameterLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && i == Buttons::BUTTON_C)
  {
    SettingsUseCases useCases(*Application::get().getSettings());
    useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Voices });
    return true;
  }

  return ParameterSelectLayout2::onButton(i, down, modifiers);
}

Parameter *MonoModeModulateableParameterLayout::getCurrentParameter() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  return eb->getSelected(vg);
}

MonoModeModulateableParameterLayout::MonoModeModulateableParameterLayout()
    : ModulateableParameterSelectLayout2()
{
}

Carousel *MonoModeModulateableParameterLayout::createCarousel(const Rect &rect)
{
  return new MonoParameterCarousel(rect);
}

bool MonoModeModulateableParameterLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  if(down && i == Buttons::BUTTON_C)
  {
    auto current = dynamic_cast<const ModulateableMonoParameter *>(
        Application::get().getPresetManager()->getEditBuffer()->getSelected(vg));
    if(current && current->getModulationSource() == MacroControls::NONE)
    {
      SettingsUseCases useCases(*Application::get().getSettings());
      useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Voices });
      return true;
    }
  }

  return ModulateableParameterSelectLayout2::onButton(i, down, modifiers);
}