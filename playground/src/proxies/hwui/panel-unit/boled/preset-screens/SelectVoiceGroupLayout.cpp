#include "SelectVoiceGroupLayout.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/controls/Button.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

SelectVoiceGroupLayout::SelectVoiceGroupLayout(PresetManagerLayout *parent)
    : DFBLayout(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
    , m_parent{ parent }
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
        m_parent->animateSelectedPreset([eb, hwui, currentVG]() {
          eb->undoableLoadSelectedPresetPartIntoPart(VoiceGroup::I, currentVG);
          hwui->setFocusAndMode({ UIFocus::Presets, UIMode::Select, UIDetail::Init });
        });
        hwui->getPanelUnit().getEditPanel().getBoled().resetOverlay();
        return true;
      case Buttons::BUTTON_C:
        m_parent->animateSelectedPreset([eb, hwui, currentVG]() {
          eb->undoableLoadSelectedPresetPartIntoPart(VoiceGroup::II, currentVG);
          hwui->setFocusAndMode({ UIFocus::Presets, UIMode::Select, UIDetail::Init });
        });
        hwui->getPanelUnit().getEditPanel().getBoled().resetOverlay();
        return true;
    }
  }

  return DFBLayout::onButton(i, down, modifiers);
}
