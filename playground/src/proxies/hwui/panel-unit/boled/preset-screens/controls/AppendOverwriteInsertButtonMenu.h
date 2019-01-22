#pragma once

#include "proxies/hwui/controls/ButtonMenu.h"
#include "device-settings/PresetStoreModeSetting.h"
#include <libundo/undo/Scope.h>

class PresetManagerLayout;
class Preset;
class Bank;

class AppendOverwriteInsertButtonMenu : public ButtonMenu
{
 private:
  typedef ButtonMenu super;

 public:
  AppendOverwriteInsertButtonMenu(PresetManagerLayout& parent, const Rect& rect);
  ~AppendOverwriteInsertButtonMenu();

  void selectButton(size_t i) override;
  void buildMenu();
  void createBankAndStore();

 private:
  PresetStoreModeSettings indexToEnum(size_t i) const;
  size_t enumToIndex(PresetStoreModeSettings i) const;

  Preset* overwritePreset(Preset* preset);
  Preset* overwritePreset(UNDO::Transaction* scope, Preset* preset);
  void insertPreset(Bank* bank, size_t pos, bool modified);
  void appendPreset(Bank* bank, bool modified);
  void pushRenameScreen();
  bool animate();

  void append(Bank* bank, bool modified);
  void insert(Bank* bank, Preset* tgtPreset, bool modified);
  void overwrite(Bank* bank, Preset* tgtPreset, bool modified);

  void executeAction();

  PresetManagerLayout& m_parent;
};
