#include <Application.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/base-unit/BaseUnitBanksMode.h>
#include <proxies/hwui/buttons.h>
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>
#include "use-cases/PresetManagerUseCases.h"

void BaseUnitBanksMode::setup()
{
  super::setup();

  setupButtonConnection(Buttons::BUTTON_MINUS, [=](auto, auto, auto state) {
    if(BaseUnitPresetsAndBanksMode::checkPanicAffenGriff(Buttons::BUTTON_MINUS, state))
      return true;

    if(state)
      installButtonRepeat([] {
        PresetManagerUseCases useCase(*Application::get().getPresetManager(), *Application::get().getSettings());
        useCase.selectPreviousBank();
      });
    else
      removeButtonRepeat();

    return true;
  });

  setupButtonConnection(Buttons::BUTTON_PLUS, [=](auto, auto, auto state) {
    if(BaseUnitPresetsAndBanksMode::checkPanicAffenGriff(Buttons::BUTTON_PLUS, state))
      return true;

    if(state)
      installButtonRepeat([] {
        PresetManagerUseCases useCase(*Application::get().getPresetManager(), *Application::get().getSettings());
        useCase.selectNextBank();
      });
    else
      removeButtonRepeat();

    return true;
  });
}
