#pragma once

#include <proxies/hwui/panel-unit/boled/preset-screens/RenameLayout.h>
#include <presets/Bank.h>

class RenameExportLayout : public RenameLayout
{
 public:
  using Callback = std::function<void(Glib::ustring, Bank *)>;
  RenameExportLayout(Bank *bank, Callback callBack);

  Glib::ustring getInitialText() const;
  void commit(const Glib::ustring &newName);

 private:
  Glib::ustring initialName;
  Bank *m_bank;
  Callback callBackAfterCommit;
};
