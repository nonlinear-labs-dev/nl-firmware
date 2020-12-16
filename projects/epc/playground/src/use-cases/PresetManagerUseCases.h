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
  enum class DropActions
  {
    Above,
    Below,
    Onto
  };

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

  void newBank(const Glib::ustring& x, const Glib::ustring& y, const Glib::ustring& name);
  void newBank(const Glib::ustring& x, const Glib::ustring& y);

  void selectPreset(const Uuid& uuid);
  void selectPreset(const Preset* preset);

  void selectBank(const Uuid& uuid);
  void selectBank(Bank* b);
  void stepBankSelection(int inc, bool shift);

  void deleteBank(const Uuid& uuid);
  void deleteBank(Bank* b);

  void selectMidiBank(Bank* b);
  void moveBankCluster(std::vector<std::string> uuids);

  void moveLeft(Bank* bank);
  void moveRight(Bank* bank);

  void sortBankNumbers();

  void dropPresets(const std::string& anchorUuid, DropActions action, const Glib::ustring& csv);

 private:
  PresetManager* m_presetManager;

  void onStore(UNDO::Transaction* transaction, Preset* preset);
  void updateSyncSettingOnPresetStore(UNDO::Transaction* transaction) const;
};
