#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/Bank.h>
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
    {
      installButtonRepeat([] {
        if(auto bank = Application::get().getPresetManager()->getSelectedBank())
          bank->selectPreviousPreset();
      });
    }
    else
    {
      removeButtonRepeat();
    }

    return true;
  });

  setupButtonConnection(BUTTON_PLUS, [=](auto, auto, auto state) {
    if(state)
    {
      installButtonRepeat([] {
        if(auto bank = Application::get().getPresetManager()->getSelectedBank())
          bank->selectNextPreset();
      });
    }
    else
    {
      removeButtonRepeat();
    }

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
