#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/Passphrase.h>
#include "PassphraseRenameLayout.h"

void PassphraseRenameLayout::commit(const Glib::ustring& newName)
{
  getSetting()->updatePassword(newName);
}

Glib::ustring PassphraseRenameLayout::getInitialText() const
{
  return getSetting()->save();
}

Passphrase* PassphraseRenameLayout::getSetting()
{
  if(Application::exists())
  {
    static auto s = Application::get().getSettings()->getSetting<Passphrase>().get();
    return s;
  }
  return nullptr;
}
