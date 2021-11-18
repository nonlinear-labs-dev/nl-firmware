#include <Application.h>
#include <device-settings/Passphrase.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/setup/PassphraseEditor.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/RenameLayout.h>
#include "PassphraseRenameLayout.h"

int PassphraseEditor::sLastSelection = 0;

PassphraseEditor::PassphraseEditor()
{
  updateOnSettingChanged();
}

PassphraseEditor::~PassphraseEditor()
{
}

void PassphraseEditor::incSetting(int inc)
{
  sLastSelection = sLastSelection + inc;
  sLastSelection = std::min(sLastSelection, 3);
  sLastSelection = std::max(sLastSelection, 0);
  updateOnSettingChanged();
}

const std::vector<Glib::ustring> &PassphraseEditor::getDisplayStrings() const
{
  static std::vector<Glib::ustring> r = { "Cancel", "Edit", "Randomize", "Default" };
  return r;
}

int PassphraseEditor::getSelectedIndex() const
{
  return sLastSelection;
}

bool PassphraseEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(MenuEditor::onButton(i, down, modifiers))
    return true;

  if(down && i == Buttons::BUTTON_ENTER)
  {
    auto setting = Application::get().getSettings()->getSetting<Passphrase>();
    switch(sLastSelection)
    {
      case 0:
        return false;
      case 1:
      {
        auto& boled = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled();
        nltools_assertAlways(boled.getOverlay() == nullptr);
        boled.setOverlay(new PassphraseRenameLayout());
        return false;
      }
      case 2:
        setting->dice();
        return false;
      case 3:
        setting->resetToDefault();
        return false;
    }
  }
  return false;
}
