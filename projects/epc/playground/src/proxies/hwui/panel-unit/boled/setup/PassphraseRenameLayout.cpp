#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/Passphrase.h>
#include "PassphraseRenameLayout.h"
#include <proxies/hwui/controls/Button.h>

void PassphraseRenameLayout::commit(const Glib::ustring& newName)
{
  getSetting()->updatePassword(newName);
}

void PassphraseRenameLayout::onTextChanged(const Glib::ustring& text)
{
  m_applyButton->setEnable(Passphrase::isValidPassword(text));
  RenameLayout::setDirty();
}

bool PassphraseRenameLayout::isTextApplicable(const Glib::ustring& text) const
{
  return Passphrase::isValidPassword(text);
}

Glib::ustring PassphraseRenameLayout::getInitialText() const
{
  return getSetting()->save();
}

Passphrase* PassphraseRenameLayout::getSetting()
{
  if(Application::exists())
  {
    static auto s = Application::get().getSettings()->getSetting<Passphrase>();
    return s;
  }
  return nullptr;
}
