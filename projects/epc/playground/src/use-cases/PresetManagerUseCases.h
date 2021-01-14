#pragma once

#include <libundo/undo/Transaction.h>
#include <tools/Uuid.h>
#include <use-cases/BankUseCases.h>
#include <libsoup/soup-message-body.h>
#include <xml/FileInStream.h>
#include <clipboard/Clipboard.h>

class PresetManager;
class Preset;
class Bank;

class PresetManagerUseCases
{
 public:
  typename std::function<void(void)> ImportVersionMismatchCallback;

  enum class DropActions
  {
    Above,
    Below,
    Onto
  };

  enum class ImportExitCode
  {
    Unsupported,
    OK
  };

  explicit PresetManagerUseCases(PresetManager* pm);

  //Preset
  //Store Actions
  void overwritePreset(const Uuid& uuid);
  void overwritePreset(Preset* preset);
  void overwritePresetWithPreset(Preset* target, Preset* source);
  void insertPresetWithUUID(Bank* bank, size_t pos, const std::string& uuid);
  void insertPreset(Bank* bank, size_t pos);
  void appendPreset(Bank* bank);
  void appendPresetWithUUID(Bank* bank, const std::string& uuid);
  void createBankAndStoreEditBuffer();
  void createBankFromPreset(const Uuid& uuid, const std::string& x, const std::string& y);
  void createBankFromPresets(const std::string& csv, const std::string& x, const std::string& y, bool directLoad);

  void newBank(const Glib::ustring& x, const Glib::ustring& y, const Glib::ustring& name);
  void newBank(const Glib::ustring& x, const Glib::ustring& y);

  void selectPreset(const Uuid& uuid, bool directLoad);
  void selectPreset(const Preset* preset, bool directLoad);

  void selectBank(const Uuid& uuid, bool directLoad);
  void selectBank(Bank* bank, bool directLoad);
  void selectBank(int idx, bool directLoad);

  void stepBankSelection(int inc, bool shift, bool directLoad);

  void setOrderNumber(Bank* b, int newOrderNumber);

  void movePresetAbove(const Uuid& presetToMoveUuid, const Uuid& presetAnchorUuid);
  void movePresetBelow(const Uuid& presetToMoveUuid, const Uuid& presetAnchorUuid);
  void movePresetTo(const Uuid& overwriteWith, const Uuid& presetToOverwrite);

  void copyPresetAbove(const Uuid& presetToCopy, const Uuid& presetAnchorUuid);
  void copyPresetBelow(const Uuid& presetToCopy, const Uuid& presetAnchorUuid);

  void insertEditBufferAbove(const Uuid& anchor, const Uuid& ebUuid);
  void insertEditBufferBelow(const Uuid& anchor, const Uuid& ebUuid);

  void deleteBank(const Uuid& uuid);
  void deleteBank(Bank* b);
  void deletePresets(const std::vector<std::string>& uuids, bool deleteBanks);

  void selectMidiBank(Bank* b);
  void moveBankCluster(std::vector<std::string> uuids);

  void moveLeft(Bank* bank);
  void moveRight(Bank* bank);

  void insertBankInCluster(Bank* bankToInsert, Bank* bankAtInsert, const Glib::ustring& directionSeenFromBankInCluster);
  void sortBankNumbers();

  void dropPresets(const std::string& anchorUuid, DropActions action, const Glib::ustring& csv);

  ImportExitCode importBackupFile(FileInStream& in);
  bool importBackupFile(SoupBuffer* buffer);
  bool loadPresetFromCompareXML(const Glib::ustring& xml);

  void moveAllBanks(float x, float y);
  void selectPreviousBank(const bool directLoad);
  void selectNextBank(const bool directLoad);

  void selectPreviousPreset(bool directLoad);
  void selectNextPreset(bool directLoad);

  void pastePresetOnBank(Bank* bank, const Preset* preset, Clipboard* pClipboard);

  void pastePresetOnPreset(Preset* target, Preset* source, Clipboard* clipboard);

  void pasteBankOnBackground(const Glib::ustring& name, const Glib::ustring& x, const Glib::ustring& y,
                             const Bank* source, Clipboard* pClipboard);

  void pastePresetOnBackground(const Glib::ustring& x, const Glib::ustring& y, Preset* source, Clipboard* clipboard);

 private:
  PresetManager* m_presetManager;

  void onStore(UNDO::Transaction* transaction, Preset* preset);
  void updateSyncSettingOnPresetStore(UNDO::Transaction* transaction) const;
};
