#pragma once
#include <proxies/hwui/panel-unit/boled/preset-screens/RenameLayout.h>

class Passphrase;

class PassphraseRenameLayout : public RenameLayout
{
 public:
  using RenameLayout::RenameLayout;

 protected:
  void commit(const Glib::ustring& newName) override;
  Glib::ustring getInitialText() const override;

  static Passphrase* getSetting();
  bool isTextApplicable(const Glib::ustring& text) const override;

 private:
  void onTextChanged(const Glib::ustring& text) override;
};
