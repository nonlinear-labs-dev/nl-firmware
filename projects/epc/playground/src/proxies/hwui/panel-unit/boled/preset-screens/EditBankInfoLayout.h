#pragma once

#include "RenameLayout.h"
#include <libundo/undo/Transaction.h>

class Bank;

class EditBankInfoLayout : public RenameLayout
{
 private:
  typedef RenameLayout super;

 public:
  EditBankInfoLayout();

 protected:
  virtual void commit(const Glib::ustring& newName) override;
  virtual Glib::ustring getInitialText() const override;

 private:
  Bank* m_currentBank = nullptr;
};
