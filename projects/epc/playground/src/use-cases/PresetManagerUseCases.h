#pragma once

#include <libundo/undo/Transaction.h>
#include <tools/Uuid.h>
class PresetManager;
class Preset;
class Bank;

class PresetManagerUseCases
{
 public:
  explicit PresetManagerUseCases(PresetManager* pm);

  //Store Actions
  void overwritePreset(Preset* preset);
  void overwritePresetWithPreset(Preset* target, Preset* source);
  void insertPresetWithUUID(Bank* bank, size_t pos, const std::string& uuid);
  void insertPreset(Bank* bank, size_t pos);
  void appendPreset(Bank* bank);
  void appendPresetWithUUID(Bank* bank, const std::string& uuid);
  void createBankAndStoreEditBuffer();
  void selectMidiBank(Bank* b);
  void deleteBank(Bank* b);

  void newBank(const Glib::ustring& x, const Glib::ustring& y, const Glib::ustring& name);
  void newBank(const Glib::ustring& x, const Glib::ustring& y);
  void renameBank(const Uuid& bankUuid, const Glib::ustring& name);
  void selectBank(const Uuid& uuid);
  void deleteBank(const Uuid& uuid);

  void moveBankCluster(std::vector<std::string> uuids);

 private:
  PresetManager* m_presetManager;

  void onStore(UNDO::Transaction* transaction, Preset* preset);
  void updateSyncSettingOnPresetStore(UNDO::Transaction* transaction) const;
};
