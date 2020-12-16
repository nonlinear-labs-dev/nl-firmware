#pragma once

#include <libundo/undo/Transaction.h>
#include <tools/Uuid.h>
#include <use-cases/BankUseCases.h>

class PresetManager;
class Preset;
class Bank;

class PresetManagerUseCases
{
 public:
  explicit PresetManagerUseCases(PresetManager* pm);

  //Preset
  //Store Actions
  void overwritePreset(Preset* preset);
  void overwritePresetWithPreset(Preset* target, Preset* source);
  void insertPresetWithUUID(Bank* bank, size_t pos, const std::string& uuid);
  void insertPreset(Bank* bank, size_t pos);
  void appendPreset(Bank* bank);
  void appendPresetWithUUID(Bank* bank, const std::string& uuid);
  void createBankAndStoreEditBuffer();

  std::unique_ptr<BankUseCases> getBankUseCase(Bank*) const;
  std::unique_ptr<BankUseCases> getBankUseCase(const Uuid& uuid) const;
  //Bank Actions Tier 1
  void newBank(const Glib::ustring& x, const Glib::ustring& y, const Glib::ustring& name);
  void newBank(const Glib::ustring& x, const Glib::ustring& y);

  void selectBank(const Uuid& uuid);
  void selectBank(Bank* b);

  void deleteBank(const Uuid& uuid);
  void deleteBank(Bank* b);

  //Bank Actions Tier 2
  void selectMidiBank(Bank* b);
  void moveBankCluster(std::vector<std::string> uuids);

 private:
  PresetManager* m_presetManager;

  void onStore(UNDO::Transaction* transaction, Preset* preset);
  void updateSyncSettingOnPresetStore(UNDO::Transaction* transaction) const;
};
