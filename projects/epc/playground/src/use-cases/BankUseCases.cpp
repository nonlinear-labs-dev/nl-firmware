#include "BankUseCases.h"
#include "presets/Bank.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"

BankUseCases::BankUseCases(Bank* bank)
    : m_bank { bank }
{
  nltools_assertAlways(m_bank != nullptr);
}

void BankUseCases::setBankComment(const Glib::ustring& comment)
{
  auto scope = m_bank->getUndoScope().startTransaction("Set Bank Comment");
  m_bank->setAttribute(scope->getTransaction(), "Comment", comment);
}

void BankUseCases::renameBank(const Glib::ustring& name)
{
  auto& undoScope = m_bank->getUndoScope();
  auto transactionScope = undoScope.startTransaction("Rename Bank '%0' to '%1'", m_bank->getName(true), name);
  auto transaction = transactionScope->getTransaction();
  m_bank->setName(transaction, name);

  if(auto pm = m_bank->getPresetManager())
  {
    pm->getEditBuffer()->undoableUpdateLoadedPresetInfo(transaction);
  }
}