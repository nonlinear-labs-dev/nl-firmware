#pragma once

#include <libundo/undo/Transaction.h>
#include <Application.h>

class LPCParameterChangeSurpressor
{
 public:
  LPCParameterChangeSurpressor(UNDO::Transaction *transaction)
      : m_transaction(transaction)
  {
    Application::get().getLPCProxy()->toggleSuppressParameterChanges(transaction);
  };

  ~LPCParameterChangeSurpressor()
  {
    Application::get().getLPCProxy()->toggleSuppressParameterChanges(m_transaction);
  }

 private:
  UNDO::Transaction *m_transaction;
};
