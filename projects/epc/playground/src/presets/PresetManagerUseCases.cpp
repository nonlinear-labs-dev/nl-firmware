#include <Application.h>
#include <parameter_declarations.h>
#include <device-settings/SyncSplitSettingUseCases.h>
#include "PresetManagerUseCases.h"
#include "presets/PresetManager.h"
#include "presets/Preset.h"
#include "presets/Bank.h"
#include "presets/EditBuffer.h"
#include "libundo/undo/Scope.h"
#include "http/UndoScope.h"
#include "libundo/undo/Transaction.h"
#include "device-settings/Settings.h"
#include "device-settings/SplitPointSyncParameters.h"
#include "parameters/SplitPointParameter.h"

PresetManagerUseCases::PresetManagerUseCases(PresetManager* pm)
    : m_presetManager { pm }
{
}

void PresetManagerUseCases::overwritePreset(Preset* preset)
{
  auto scope = Application::get().getUndoScope()->startTransaction("Overwrite preset '%0'", preset->getName());
  auto transaction = scope->getTransaction();

  if(auto bank = dynamic_cast<Bank*>(preset->getParent()))
  {
    preset->copyFrom(transaction, m_presetManager->getEditBuffer());
    m_presetManager->getEditBuffer()->undoableLoad(transaction, preset, false);
    bank->selectPreset(transaction, preset->getUuid());
    m_presetManager->selectBank(transaction, bank->getUuid());

    onStore(transaction, preset);

    assert(m_presetManager->getSelectedBank() == bank);
    assert(bank->getSelectedPreset() == preset);
    assert(m_presetManager->getEditBuffer()->getUUIDOfLastLoadedPreset() == preset->getUuid());
  }
}

void PresetManagerUseCases::overwritePresetWithPreset(Preset* target, Preset* source)
{
  auto scope = Application::get().getUndoScope()->startTransaction("Overwrite preset '%0' with '%1'", target->getName(),
                                                                   source->getName());
  if(auto targetBank = dynamic_cast<Bank*>(target->getParent()))
  {
    auto transaction = scope->getTransaction();
    target->copyFrom(transaction, source);
    targetBank->selectPreset(transaction, target->getUuid());
    m_presetManager->selectBank(transaction, targetBank->getUuid());

    onStore(transaction, target);

    assert(m_presetManager->getSelectedBank() == targetBank);
    assert(targetBank->getSelectedPreset() == target);
  }
}

void PresetManagerUseCases::insertPresetWithUUID(Bank* bank, size_t pos, const std::string& uuid)
{
  Uuid id;

  if(uuid.empty())
  {
    id.generate();
  }
  else
  {
    id = uuid;
  }

  auto pm = Application::get().getPresetManager();
  auto scope = Application::get().getUndoScope()->startTransaction("Insert preset at position %0", pos + 1);
  auto transaction = scope->getTransaction();
  auto ebIsModified = m_presetManager->getEditBuffer()->isModified();

  auto preset = bank->insertAndLoadPreset(transaction, pos, std::make_unique<Preset>(bank, *pm->getEditBuffer(), id));

  m_presetManager->selectBank(transaction, bank->getUuid());
  bank->selectPreset(transaction, preset->getUuid());

  if(ebIsModified)
    preset->guessName(transaction);

  onStore(transaction, preset);

  assert(m_presetManager->getSelectedBank() == bank);
  assert(bank->getSelectedPreset() == preset);
  assert(m_presetManager->getEditBuffer()->getUUIDOfLastLoadedPreset() == preset->getUuid());
}

void PresetManagerUseCases::insertPreset(Bank* bank, size_t pos)
{
  insertPresetWithUUID(bank, pos, "");
}

void PresetManagerUseCases::appendPreset(Bank* bank)
{
  insertPresetWithUUID(bank, bank->getNumPresets(), "");
}

void PresetManagerUseCases::appendPreset(Bank* bank, Preset* preset)
{
  auto scope = Application::get().getUndoScope()->startTransaction("Append preset to Bank '%0'", bank->getName(true));
  auto transaction = scope->getTransaction();
  auto ebIsModified = m_presetManager->getEditBuffer()->isModified();

  auto newPreset = std::make_unique<Preset>(bank, *preset, true);
  auto storedPreset = bank->insertAndLoadPreset(transaction, bank->getNumPresets(), std::move(newPreset));

  m_presetManager->selectBank(transaction, bank->getUuid());
  bank->selectPreset(transaction, storedPreset->getUuid());

  if(ebIsModified)
    storedPreset->guessName(transaction);

  onStore(transaction, storedPreset);

  assert(m_presetManager->getSelectedBank() == bank);
  assert(bank->getSelectedPreset() == storedPreset);
  assert(m_presetManager->getEditBuffer()->getUUIDOfLastLoadedPreset() == storedPreset->getUuid());
}

void PresetManagerUseCases::appendPresetWithUUID(Bank* bank, const std::string& uuid)
{
  insertPresetWithUUID(bank, bank->getNumPresets(), uuid);
}

void PresetManagerUseCases::createBankAndStoreEditBuffer()
{
  auto& scope = m_presetManager->getUndoScope();
  auto transactionScope = scope.startTransaction("Create Bank and Store Preset");
  auto transaction = transactionScope->getTransaction();
  auto bank = m_presetManager->addBank(transaction);
  auto preset
      = bank->appendAndLoadPreset(transaction, std::make_unique<Preset>(bank, *m_presetManager->getEditBuffer()));
  bank->selectPreset(transaction, preset->getUuid());
  m_presetManager->selectBank(transaction, bank->getUuid());

  onStore(transaction, preset);
}

void PresetManagerUseCases::onStore(UNDO::Transaction* transaction, Preset* preset)
{
  if(preset->getType() == SoundType::Split)
  {
    updateSyncSettingOnPresetStore(transaction);
  }

  m_presetManager->onPresetStored();
}

void PresetManagerUseCases::updateSyncSettingOnPresetStore(UNDO::Transaction* transaction) const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(auto s = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::I }))
  {
    if(s->hasOverlap())
    {
      SyncSplitSettingUseCases::get().disableSyncSetting(transaction);
    }
    else
    {
      SyncSplitSettingUseCases::get().enableSyncSetting(transaction);
    }
  }
}

void PresetManagerUseCases::selectMidiBank(Bank* b)
{
  if(b)
  {
    auto& scope = m_presetManager->getUndoScope();
    auto transactionScope = scope.startTransaction("Select Bank '%0' for Midi Program-Changes", b->getName(true));
    auto transaction = transactionScope->getTransaction();
    m_presetManager->selectMidiBank(transaction, b->getUuid());
  }
  else
  {
    auto& scope = m_presetManager->getUndoScope();
    auto transactionScope = scope.startTransaction("Remove Bank for Midi Program-Changes");
    auto transaction = transactionScope->getTransaction();
    m_presetManager->selectMidiBank(transaction, "");
  }
}

void PresetManagerUseCases::deleteBank(Bank* b)
{
  if(b)
  {
    auto& scope = m_presetManager->getUndoScope();
    auto transScope = scope.startTransaction("Delete Bank '%0'", b->getName(true));
    auto transaction = transScope->getTransaction();

    //Update Bank Selection
    if(m_presetManager->getSelectedBankUuid() == b->getUuid())
      if(!m_presetManager->selectPreviousBank(transaction))
        m_presetManager->selectNextBank(transaction);

    //Clear Midi bank
    if(m_presetManager->getMidiSelectedBank() == b->getUuid())
      m_presetManager->selectMidiBank(transaction, Uuid::none());

    m_presetManager->deleteBank(transaction, b->getUuid());
  }
}
