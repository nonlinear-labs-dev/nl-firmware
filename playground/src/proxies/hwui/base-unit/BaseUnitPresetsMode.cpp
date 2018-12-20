#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/PresetBank.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/AutoLoadSelectedPreset.h>
#include <playground.h>
#include <proxies/hwui/base-unit/BaseUnitPresetsMode.h>
#include <proxies/hwui/buttons.h>

void BaseUnitPresetsMode::setup()
{
  super::setup();

  setupButtonConnection(BUTTON_MINUS, [=](auto, auto, auto state) {
    if(state)
      if(auto bank = Application::get().getPresetManager()->getSelectedBank())
        bank->undoableIncPresetSelection(-1, ButtonModifiers());

    return true;
  });

  setupButtonConnection(BUTTON_PLUS, [=](auto, auto, auto state) {
    if(state)
      if(auto bank = Application::get().getPresetManager()->getSelectedBank())
        bank->undoableIncPresetSelection(1, ButtonModifiers());

    return true;
  });
}

void BaseUnitPresetsMode::onFuncButtonDown()
{
  auto &app = Application::get();
  auto eb = app.getPresetManager()->getEditBuffer();

  if(!app.getSettings()->getSetting<AutoLoadSelectedPreset>()->get())
    eb->undoableLoadSelectedPreset();
  else if(eb->isModified())
    eb->undoableLoadSelectedPreset();
}
