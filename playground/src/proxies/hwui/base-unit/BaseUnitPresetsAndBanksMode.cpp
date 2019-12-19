#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>
#include <device-settings/BaseUnitUIMode.h>
#include <device-settings/Settings.h>
#include <device-settings/AutoLoadSelectedPreset.h>
#include <playground.h>
#include <proxies/hwui/base-unit/BaseUnitPresetsAndBanksMode.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUIEnums.h>

BaseUnitPresetsAndBanksMode::BaseUnitPresetsAndBanksMode()
    : m_modeButtonHandler(std::bind(&BaseUnitPresetsAndBanksMode::modeButtonShortPress, this),
                          std::bind(&BaseUnitPresetsAndBanksMode::modeButtonLongPress, this))
    , m_funcButtonHandler([] {},
                          [] { cycle(); })
{
}

void BaseUnitPresetsAndBanksMode::setup()
{
  setupButtonConnection(Buttons::BUTTON_FUNCTION, [=](auto, auto, auto state) {
    if(state)
      onFuncButtonDown();

    m_funcButtonHandler.onButtonEvent(state);
    return true;
  });

  setupButtonConnection(Buttons::BUTTON_MODE, [=](auto, auto, auto state) {
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

void BaseUnitPresetsAndBanksMode::cycle()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto setting = Application::get().getSettings()->getSetting<AutoLoadSelectedPreset>();

  if(eb->getType() == SoundType::Single)
  {
    switch(setting->get())
    {
      case LoadPresetMode::PresetSelect:
      case LoadPresetMode::PartSelect:
        setting->set(LoadPresetMode::DirectLoad);
        break;
      case LoadPresetMode::DirectLoad:
        setting->set(LoadPresetMode::PresetSelect);
        break;
    }
  }
  else
  {
    switch(setting->get())
    {
      case LoadPresetMode::PartSelect:
        setting->set(LoadPresetMode::PresetSelect);
        break;
      case LoadPresetMode::PresetSelect:
        setting->set(LoadPresetMode::DirectLoad);
        break;
      case LoadPresetMode::DirectLoad:
        setting->set(LoadPresetMode::PartSelect);
        break;
    }
  }
}
