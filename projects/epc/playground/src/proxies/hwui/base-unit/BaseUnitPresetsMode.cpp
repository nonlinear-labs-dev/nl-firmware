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
#include "use-cases/PresetManagerUseCases.h"
#include "use-cases/EditBufferUseCases.h"

void BaseUnitPresetsMode::setup()
{
  super::setup();

  setupButtonConnection(Buttons::BUTTON_MINUS,
                        [=](auto, auto, auto state)
                        {
                          if(BaseUnitPresetsAndBanksMode::checkPanicAffenGriff(Buttons::BUTTON_MINUS, state))
                            return true;

                          if(state)
                            installButtonRepeat(
                                []()
                                {
                                  auto pm = Application::get().getPresetManager();
                                  PresetManagerUseCases useCase(*pm, *Application::get().getSettings());
                                  useCase.selectPreviousPreset();
                                });
                          else
                            removeButtonRepeat();

                          return true;
                        });

  setupButtonConnection(Buttons::BUTTON_PLUS,
                        [=](auto, auto, auto state)
                        {
                          if(BaseUnitPresetsAndBanksMode::checkPanicAffenGriff(Buttons::BUTTON_MINUS, state))
                            return true;

                          if(state)
                            installButtonRepeat(
                                []()
                                {
                                  auto pm = Application::get().getPresetManager();
                                  PresetManagerUseCases useCase(*pm, *Application::get().getSettings());
                                  useCase.selectNextPreset();
                                });
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
  auto currentVoiceGroup = app.getVGManager()->getCurrentVoiceGroup();

  EditBufferUseCases useCase(*eb);
  if(app.getVGManager()->isInLoadToPart() && eb->isDual())
  {
    if(auto preset = eb->getParent()->getSelectedPreset())
      useCase.loadToPart(preset, VoiceGroup::I, currentVoiceGroup);
  }
  else
  {
    useCase.load(eb->getParent()->getSelectedPreset());
  }
}
