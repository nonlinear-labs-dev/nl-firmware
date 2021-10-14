#pragma once

#include "proxies/hwui/controls/ButtonMenu.h"
#include <device-settings/PresetStoreModeSetting.h>

class PresetManagerLayout;
class Preset;
class Bank;

namespace UNDO
{
  class Transaction;
}

class AppendOverwriteInsertButtonMenu : public ButtonMenu
{
 private:
  typedef ButtonMenu super;

 public:
  AppendOverwriteInsertButtonMenu(PresetManagerLayout& parent, const Rect& rect);
  ~AppendOverwriteInsertButtonMenu() override;

  void selectButton(size_t i) override;
  void buildMenu();

 private:
  PresetStoreModeSettings indexToEnum(size_t i) const;
  size_t enumToIndex(PresetStoreModeSettings i) const;

  void pushRenameScreen(Preset* target);
  bool animateSelectedPreset();
  bool animateSomePreset(Preset* target);

  void executeAction();

  PresetManagerLayout& m_parent;
};
