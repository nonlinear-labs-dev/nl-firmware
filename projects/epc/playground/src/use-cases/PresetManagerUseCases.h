#pragma once

#include <libundo/undo/Transaction.h>
#include <tools/Uuid.h>
#include <use-cases/BankUseCases.h>
#include <libsoup/soup-message-body.h>
#include <xml/FileInStream.h>
#include <clipboard/Clipboard.h>
#include <filesystem>
#include <proxies/audio-engine/AudioEngineProxy.h>

class PresetManager;
class Preset;
class Bank;
class Settings;

class PresetManagerUseCases
{
 public:
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

  explicit PresetManagerUseCases(PresetManager& pm, Settings& settings);

  void appendPreset(Bank* bank, Preset* preset);
  Bank* createBankAndStoreEditBuffer();
  void createBankFromPreset(const Uuid& uuid, const std::string& x, const std::string& y);
  void createBankFromPresets(const std::string& csv, const std::string& x, const std::string& y);

  Bank* newBank(const Glib::ustring& x, const Glib::ustring& y, const std::optional<Glib::ustring>& name);
  Bank* newBank(const Glib::ustring& x, const Glib::ustring& y, const Uuid& uuid);

  void selectPreset(const Uuid& uuid);
  void selectPreset(const Preset* preset);

  void selectBank(const Uuid& uuid);
  void selectBank(Bank* bank);
  void selectBank(int idx);

  void stepBankSelection(int inc, bool shift);

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

  void clear();

  using StartProgressIndication = std::function<void()>;
  using UpdateProgressIndication = std::function<void(const std::string&)>;
  using FinishProgressIndication = std::function<void()>;

  struct ProgressIndication
  {
    static ProgressIndication getMock()
    {
      ProgressIndication ret {};
      ret._start = []() {};
      ret._update = [](auto) {};
      ret._finish = []() {};
      return ret;
    }

    void start() const
    {
      if(_start)
        _start();
    }

    void update(const std::string& msg) const
    {
      if(_update)
        _update(msg);
    }

    void finish() const
    {
      if(_finish)
        _finish();
    }

    StartProgressIndication _start = std::function<void(void)>();
    UpdateProgressIndication _update = std::function<void(std::string)>();
    FinishProgressIndication _finish = std::function<void(void)>();
  };

  PresetManagerUseCases::ImportExitCode importBackupFile(FileInStream& in, const ProgressIndication& progress,
                                                         AudioEngineProxy& ae);
  bool importBackupFile(SoupBuffer* buffer, ProgressIndication progress, AudioEngineProxy& ae);
  bool importBackupFile(UNDO::Transaction* transaction, InStream& in, const ProgressIndication& progress,
                        AudioEngineProxy& aeProxy);

  bool loadPresetFromCompareXML(const Glib::ustring& xml);

  void moveAllBanks(float x, float y);
  void selectPreviousBank();
  void selectNextBank();

  void selectPreviousPreset();
  void selectNextPreset();

  void pastePresetOnBank(Bank* bank, const Preset* preset, Clipboard* pClipboard);

  void pastePresetOnPreset(Preset* target, Preset* source, Clipboard* clipboard);

  void pasteBankOnBackground(const Glib::ustring& name, const Glib::ustring& x, const Glib::ustring& y,
                             const Bank* source, Clipboard* pClipboard);

  void pastePresetOnBackground(const Glib::ustring& x, const Glib::ustring& y, Preset* source, Clipboard* clipboard);
  Bank* importBankFromPath(const std::filesystem::directory_entry& file,
                           const std::function<void(const std::string&)>& progress);
  Bank* importBankFromStream(InStream& stream, int x, int y, const Glib::ustring& fileName,
                             const std::function<void(const std::string&)>& progress);

  Bank* addBank();

 private:
  [[nodiscard]] bool isDirectLoadActive() const;

  PresetManager& m_presetManager;
  Settings& m_settings;
};
