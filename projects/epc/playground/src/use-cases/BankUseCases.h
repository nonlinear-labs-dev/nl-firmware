#pragma once
#include <glibmm/ustring.h>
#include <tools/Uuid.h>
#include <functional>

class Bank;
class Preset;
class PresetManager;
class Settings;

class BankUseCases
{
 public:
  explicit BankUseCases(Bank* bank, Settings& settings);

  void selectPreset(int pos);
  void stepPresetSelection(int inc);
  void selectFirstOrLastPreset(int direction);

  void renameBank(const Glib::ustring& name);
  void setBankComment(const Glib::ustring& comment);

  void moveBank(const std::string& x, const std::string& y);

  void dropPresets(const std::string& csv);
  void dropBank(const Bank* b);
  void dropBankOnPreset(const Bank* b, const Uuid& presetAnchor);

  void deletePreset(const Preset* p);
  void deletePreset(const Uuid& uuid);

  void setAttribute(const Glib::ustring& key, const Glib::ustring& value);

  void setCollapsed(bool b);

  void exportBankToFile(const std::string& basicString);

  void insertBank(Bank* toInsert, size_t insertPosition);

  Preset* insertEditBufferAtPosition(int anchor);
  Preset* appendEditBuffer();
  Preset* appendEditBufferAsPresetWithUUID(Uuid uuid);
  Preset* insertEditBufferAsPresetWithUUID(size_t pos, Uuid uuid);

 private:
  [[nodiscard]] bool isDirectLoadActive() const;
  [[nodiscard]] PresetManager* getPresetManager() const;

  Bank* m_bank;
  Settings& m_settings;
};
