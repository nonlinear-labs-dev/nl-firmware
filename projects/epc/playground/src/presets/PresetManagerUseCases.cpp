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

void PresetManagerUseCases::newBank(const Glib::ustring& x, const Glib::ustring& y, const Glib::ustring& name)
{
  auto scope = m_presetManager->getUndoScope().startTransaction("New Bank");
  auto transaction = scope->getTransaction();
  auto bank = m_presetManager->addBank(transaction);
  bank->setX(transaction, x);
  bank->setY(transaction, y);
  bank->setName(scope->getTransaction(), name);
  m_presetManager->selectBank(scope->getTransaction(), bank->getUuid());
}

void PresetManagerUseCases::newBank(const Glib::ustring& x, const Glib::ustring& y)
{
  auto scope = m_presetManager->getUndoScope().startTransaction("New Bank");
  auto transaction = scope->getTransaction();
  auto bank = m_presetManager->addBank(transaction);
  bank->setX(transaction, x);
  bank->setY(transaction, y);
  auto preset
      = bank->appendAndLoadPreset(transaction, std::make_unique<Preset>(bank, m_presetManager->getEditBuffer(), false));
  bank->selectPreset(transaction, preset->getUuid());
  m_presetManager->selectBank(transaction, bank->getUuid());
}

void PresetManagerUseCases::renameBank(const Uuid& bankUuid, const Glib::ustring& name)
{
  if(auto bank = m_presetManager->findBank(bankUuid))
  {
    auto& undoScope = m_presetManager->getUndoScope();
    auto transactionScope = undoScope.startTransaction("Rename Bank '%0' to '%1'", bank->getName(true), name);
    auto transaction = transactionScope->getTransaction();
    bank->setName(transaction, name);
    m_presetManager->getEditBuffer()->undoableUpdateLoadedPresetInfo(transaction);
  }
}

void PresetManagerUseCases::selectBank(const Uuid& uuid)
{
  if(auto bank = m_presetManager->findBank(uuid))
  {
    auto& undoScope = m_presetManager->getUndoScope();
    auto transactionScope = undoScope.startTransaction("Select Bank '%0'", bank->getName(true));
    auto transaction = transactionScope->getTransaction();
    m_presetManager->selectBank(transaction, uuid);
  }
}

void PresetManagerUseCases::deleteBank(const Uuid& uuid)
{
  if(auto bank = m_presetManager->findBank(uuid))
  {
    auto scope = m_presetManager->getUndoScope().startTransaction("Delete Bank '%0'", bank->getName(true));
    auto transaction = scope->getTransaction();

    if(m_presetManager->getSelectedBankUuid() == uuid)
      if(!m_presetManager->selectPreviousBank(transaction))
        m_presetManager->selectNextBank(transaction);

    m_presetManager->deleteBank(transaction, uuid);
  }
}

void PresetManagerUseCases::moveBankCluster(std::vector<std::string> uuids)
{
  auto scope = m_presetManager->getUndoScope().startTransaction("Moved Banks");
  auto transaction = scope->getTransaction();

  nltools_assertAlways(uuids.size() % 3 == 0);

  for(auto i = uuids.begin(); i != uuids.end();)
  {
    if(auto selBank = m_presetManager->findBank(*(i++)))
    {
      auto x = *(i++);
      auto y = *(i++);
      selBank->setX(transaction, x);
      selBank->setY(transaction, y);
    }
    else
    {
      std::advance(i, 2);
    }
  }
}