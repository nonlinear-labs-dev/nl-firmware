#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>
#include <playground.h>
#include <proxies/hwui/base-unit/BaseUnitPresetsMode.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>

void BaseUnitPresetsMode::setup()
{
  super::setup();

  setupButtonConnection(Buttons::BUTTON_MINUS, [=](auto, auto, auto state) {
    auto pm = Application::get().getPresetManager();
    PresetManagerUseCases useCase(pm);
    auto directLoad = Application::get().getSettings()->getSetting<DirectLoadSetting>()->get();

    if(state)
      installButtonRepeat([&] { useCase.selectPreviousPreset(directLoad); });
    else
      removeButtonRepeat();

    return true;
  });

  setupButtonConnection(Buttons::BUTTON_PLUS, [=](auto, auto, auto state) {
    auto pm = Application::get().getPresetManager();
    PresetManagerUseCases useCase(pm);
    auto directLoad = Application::get().getSettings()->getSetting<DirectLoadSetting>()->get();

    if(state)
      installButtonRepeat([&] { useCase.selectNextPreset(directLoad); });
    else
      removeButtonRepeat();

    return true;
  });
}

void BaseUnitPresetsMode::onFuncButtonDown()
{
  auto &app = Application::get();
  auto hwui = Application::get().getHWUI();
  auto eb = app.getPresetManager()->getEditBuffer();
  auto currentVoiceGroup = hwui->getCurrentVoiceGroup();

  EditBufferUseCases useCase(eb);
  if(hwui->isInLoadToPart() && eb->isDual())
  {
    useCase.loadSelectedSinglePresetIntoDualSound(currentVoiceGroup);
  }
  else
  {
    useCase.autoLoadSelectedPreset();
  }
}
