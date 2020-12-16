#pragma once

#include <glibmm/ustring.h>
class Bank;

class BankUseCases
{
 public:
  explicit BankUseCases(Bank* bank);

  void renameBank(const Glib::ustring& name);
  void setBankComment(const Glib::ustring& comment);

 private:
  Bank* m_bank;
};
