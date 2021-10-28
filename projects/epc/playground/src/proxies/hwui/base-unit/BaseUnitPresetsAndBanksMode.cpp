#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>
#include <device-settings/BaseUnitUIMode.h>
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>
#include <playground.h>
#include <proxies/hwui/base-unit/BaseUnitPresetsAndBanksMode.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUIEnums.h>
#include <use-cases/DirectLoadUseCases.h>
#include <use-cases/SettingsUseCases.h>

BaseUnitPresetsAndBanksMode::BaseUnitPresetsAndBanksMode()
    : m_modeButtonHandler([this] { modeButtonShortPress(); },
                          [this] { modeButtonLongPress(); })
    , m_funcButtonHandler([] {}, [] {
                            auto setting = Application::get().getSettings()->getSetting<DirectLoadSetting>().get();
                            DirectLoadUseCases useCase(setting);
                            useCase.toggleDirectLoadFromBaseUnit();
                          })
{
  for(auto b : { Buttons::BUTTON_MINUS, Buttons::BUTTON_PLUS, Buttons::BUTTON_MODE, Buttons::BUTTON_FUNCTION })
    m_buttonStates.emplace(b, false);
}

void BaseUnitPresetsAndBanksMode::setup()
{
  setupButtonConnection(Buttons::BUTTON_FUNCTION, [=](auto, auto, auto state) {
    if(checkPanicAffenGriff(Buttons::BUTTON_FUNCTION, state))
      return true;

    if(state)
      onFuncButtonDown();

    m_funcButtonHandler.onButtonEvent(state);
    return true;
  });

  setupButtonConnection(Buttons::BUTTON_MODE, [=](auto, auto, auto state) {
    if(checkPanicAffenGriff(Buttons::BUTTON_MODE, state))
      return true;

    m_modeButtonHandler.onButtonEvent(state);
    return true;
  });
}

void BaseUnitPresetsAndBanksMode::modeButtonShortPress()
{
  auto s = Application::get().getSettings()->getSetting<BaseUnitUIMode>();

  if(s->get() == BaseUnitUIModes::Presets)
    s->set(BaseUnitUIModes::Banks);
  else
    s->set(BaseUnitUIModes::Presets);
}

void BaseUnitPresetsAndBanksMode::modeButtonLongPress()
{
  Application::get().getSettings()->getSetting<BaseUnitUIMode>()->set(BaseUnitUIModes::Play);
}

void BaseUnitPresetsAndBanksMode::onFuncButtonDown()
{
}

void BaseUnitPresetsAndBanksMode::installButtonRepeat(const std::function<void()>& cb)
{
  m_buttonRepeat = std::make_unique<ButtonRepeat>(cb);
}

void BaseUnitPresetsAndBanksMode::removeButtonRepeat()
{
  m_buttonRepeat.reset();
}

bool BaseUnitPresetsAndBanksMode::checkPanicAffenGriff(Buttons b, bool state)
{
    m_buttonStates[b] = state;
    if(std::all_of(m_buttonStates.cbegin(), m_buttonStates.cend(), [](auto x) { return x.second == true; }))
    {
        SettingsUseCases::panicAudioEngine();
        return true;
    }
    return false;
}
