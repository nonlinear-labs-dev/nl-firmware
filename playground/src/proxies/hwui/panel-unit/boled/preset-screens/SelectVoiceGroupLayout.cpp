#include "SelectVoiceGroupLayout.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/controls/Button.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

SelectVoiceGroupLayout::SelectVoiceGroupLayout()
    : DFBLayout(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
{
  addControl(new Label("Load Part I or II from Preset?", { 0, 0, 256, 64 }));
  addControl(new Button("I", Buttons::BUTTON_B));
  addControl(new Button("II", Buttons::BUTTON_C));
}

bool SelectVoiceGroupLayout::onButton(Buttons i, bool down, ::ButtonModifiers modifiers)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto hwui = Application::get().getHWUI();
  auto currentVG = Application::get().getHWUI()->getCurrentVoiceGroup();

  if(down)
  {
    switch(i)
    {
      case Buttons::BUTTON_B:
        eb->undoableLoadSelectedPresetPartIntoPart(VoiceGroup::I, currentVG);
        hwui->setFocusAndMode({ UIFocus::Presets, UIMode::Select, UIDetail::Init });
        return true;
      case Buttons::BUTTON_C:
        eb->undoableLoadSelectedPresetPartIntoPart(VoiceGroup::II, currentVG);
        hwui->setFocusAndMode({ UIFocus::Presets, UIMode::Select, UIDetail::Init });
        return true;
    }
  }

  return DFBLayout::onButton(i, down, modifiers);
}
