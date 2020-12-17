#pragma once
#include <glibmm/ustring.h>
#include <tools/Uuid.h>

class Bank;
class Preset;

class BankUseCases
{
 public:
  explicit BankUseCases(Bank* bank);

  void renameBank(const Glib::ustring& name);
  void setBankComment(const Glib::ustring& comment);
  void stepPresetSelection(int inc);
  void moveBank(const std::string& x, const std::string& y);

  void dropPresets(const std::string& csv);
  void dropBank(const Bank* b);
  void dropBankOnPreset(const Bank* b, const Uuid& presetAnchor);

  void deletePreset(const Preset* p);
  void deletePreset(const Uuid& uuid);

 private:
  Bank* m_bank;
};
