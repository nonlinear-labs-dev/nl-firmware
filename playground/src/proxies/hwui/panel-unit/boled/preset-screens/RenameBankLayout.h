#pragma once

#include "RenameLayout.h"

namespace UNDO
{
  class Transaction;
}

class Bank;

class RenameBankLayout : public RenameLayout
{
 private:
  typedef RenameLayout super;

 public:
  RenameBankLayout(UNDO::Transaction* transaction = nullptr);

 private:
  virtual void commit(const Glib::ustring& newName) override;
  virtual Glib::ustring getInitialText() const override;

  Bank* m_currentBank = nullptr;
  UNDO::Transaction* m_transaction;
};
