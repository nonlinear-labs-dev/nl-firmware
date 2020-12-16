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

  void deletePreset(const Preset* p);
  void deletePreset(const Uuid& uuid);

 private:
  Bank* m_bank;
};
