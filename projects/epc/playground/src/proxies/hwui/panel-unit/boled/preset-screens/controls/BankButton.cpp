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

BankButton::BankButton(const Rect& pos, bool bankFocus)
    : ControlWithChildren(pos)
    , m_bankFocus(bankFocus)
{
  auto& famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();
  famSetting.onChange(sigc::mem_fun(this, &BankButton::onFocusAndModeChanged));
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

void BankButton::installDefault(FocusAndMode focusAndMode)
{
  addControl(new Button("I / II", { 4, 0, 50, 11 }));
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

  auto toggleVoiceGroup = []
  {
    VoiceGroupUseCases vgUseCases(Application::get().getVGManager(),
                                  Application::get().getPresetManager()->getEditBuffer());
    vgUseCases.toggleVoiceGroupSelectionSilent();
  };

  m_buttonAHandler = std::make_unique<ShortVsLongPress>(toggleBankFocus, toggleVoiceGroup);
}

void BankButton::installSmall(FocusAndMode focusAndMode)
{
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

void BankButton::onFocusAndModeChanged(const Setting* s)
{
  clear();

  if(auto fam = dynamic_cast<const FocusAndModeSetting*>(s))
  {
    auto focusAndMode = fam->getState();
    if(focusAndMode.mode == UIMode::Store || focusAndMode.mode == UIMode::Edit)
      installSmall(focusAndMode);
    else
      installDefault(focusAndMode);
  }
}
