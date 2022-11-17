#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/Button.h>
#include <sigc++/adaptors/hide.h>
#include "BankButton.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/VoiceGroupUseCases.h"
#include <device-settings/Settings.h>
#include <device-settings/FocusAndModeSetting.h>

auto getSoundType()
{
  return Application::get().getPresetManager()->getEditBuffer()->getType();
}

BankButton::BankButton(const Rect& pos, bool bankFocus)
    : ControlWithChildren(pos)
    , m_bankFocus(bankFocus)
{
  m_soundTypeChanged = Application::get().getPresetManager()->getEditBuffer()->onSoundTypeChanged(
      sigc::hide(sigc::mem_fun(this, &BankButton::bruteForce)));
}

BankButton::~BankButton()
{
  m_soundTypeChanged.disconnect();
}

bool BankButton::onButton(Buttons b, bool down, ButtonModifiers modifiers)
{
  if(m_buttonAHandler && b == Buttons::BUTTON_A)
  {
    m_buttonAHandler->onButtonEvent(down);
    return true;
  }
  return false;
}

void BankButton::bruteForce()
{
  clear();

  auto& famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();
  auto mode = famSetting.getState().mode;

  switch(getSoundType())
  {
    case SoundType::Single:
      installSingle();
      break;
    case SoundType::Layer:
    case SoundType::Split:
      if(mode == UIMode::Select)
        installDual();
      else
        installSingle();
      break;
  }
}

void BankButton::installSingle()
{
  auto& famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();
  auto focusAndMode = famSetting.getState();
  auto bankbutton = addControl(new Button("Bank", { 0, 15, 58, 11 }));
  bankbutton->setHighlight(focusAndMode.focus == UIFocus::Banks);

  auto toggleBankFocus = [this]
  {
    SettingsUseCases useCases(*Application::get().getSettings());

    if(m_bankFocus)
      useCases.setFocusAndMode({ UIFocus::Presets, UIMode::Unchanged, UIDetail::Unchanged });
    else
      useCases.setFocusAndMode({ UIFocus::Banks, UIMode::Unchanged, UIDetail::Unchanged });
  };

  m_buttonAHandler = std::make_unique<ShortVsLongPress>(toggleBankFocus, toggleBankFocus);
}

void BankButton::installDual()
{
  auto& famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();
  auto focusAndMode = famSetting.getState();
  auto bankbutton = addControl(new Button("Bank", { 4, 0, 50, 11 }));
  bankbutton->setHighlight(focusAndMode.focus == UIFocus::Banks);

  addControl(new Button("I / II", { 0, 15, 58, 11 }));

  auto toggleBankFocus = [this]
  {
    SettingsUseCases useCases(*Application::get().getSettings());

    if(m_bankFocus)
      useCases.setFocusAndMode({ UIFocus::Presets, UIMode::Unchanged, UIDetail::Unchanged });
    else
      useCases.setFocusAndMode({ UIFocus::Banks, UIMode::Unchanged, UIDetail::Unchanged });
  };

  m_buttonAHandler = std::make_unique<ShortVsLongPress>(
      []
      {
        VoiceGroupUseCases vgUseCases(Application::get().getVGManager(),
                                      Application::get().getPresetManager()->getEditBuffer());
        vgUseCases.toggleVoiceGroupSelectionSilent();
      },
      toggleBankFocus);
}
