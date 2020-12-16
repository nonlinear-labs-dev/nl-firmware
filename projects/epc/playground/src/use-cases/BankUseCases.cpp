#include "BankUseCases.h"
#include "presets/Bank.h"
#include "presets/PresetManager.h"
#include "presets/Preset.h"
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

void BankUseCases::stepPresetSelection(int inc)
{
  if(auto presetManager = m_bank->getPresetManager())
  {
    auto current = m_bank->getSelectedPreset();
    auto currentPos = m_bank->getPresetPosition(current);
    auto presetPosToSelect = std::max(0, std::min<int>(m_bank->getNumPresets() - 1, currentPos + inc));

    if(auto presetToSelect = m_bank->getPresetAt(presetPosToSelect))
    {
      auto name = presetToSelect->buildUndoTransactionTitle("Select Preset");
      auto scope = presetManager->getUndoScope().startContinuousTransaction(presetManager, std::chrono::hours(1), name);
      m_bank->selectPreset(scope->getTransaction(), presetToSelect->getUuid());
    }
  }
}

void BankUseCases::deletePreset(const Preset* p)
{
  if(p)
  {
    deletePreset(p->getUuid());
  }
}
 
void BankUseCases::deletePreset(const Uuid& uuid)
{
  if(auto preset = m_bank->findPreset(uuid))
  {
    auto scope = m_bank->getUndoScope().startTransaction("Delete preset '%0'", preset->getName());
    m_bank->deletePreset(scope->getTransaction(), preset->getUuid());

    if(auto pm = m_bank->getPresetManager())
    {
      pm->getEditBuffer()->undoableUpdateLoadedPresetInfo(scope->getTransaction());
    }
  }
}