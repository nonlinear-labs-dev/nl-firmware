#include <Application.h>
#include <parameter_declarations.h>
#include <device-settings/SyncSplitSettingUseCases.h>
#include <presets/ClusterEnforcement.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
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

void PresetManagerUseCases::selectBank(Bank* bank)
{
  if(bank)
  {
    selectBank(bank->getUuid());
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
    m_presetManager->getEditBuffer()->undoableUpdateLoadedPresetInfo(transaction);
  }
}

std::unique_ptr<BankUseCases> PresetManagerUseCases::getBankUseCase(Bank* b) const
{
  if(b)
  {
    return std::move(std::make_unique<BankUseCases>(b));
  }
  return nullptr;
}

std::unique_ptr<BankUseCases> PresetManagerUseCases::getBankUseCase(const Uuid& uuid) const
{
  if(auto b = m_presetManager->findBank(uuid))
  {
    return std::move(std::make_unique<BankUseCases>(b));
  }
  return nullptr;
}

void PresetManagerUseCases::selectPreset(const Uuid& uuid)
{
  if(auto preset = m_presetManager->findPreset(uuid))
  {
    selectPreset(preset);
  }
}

void PresetManagerUseCases::selectPreset(const Preset* preset)
{
  if(preset)
  {
    const auto& presetUuid = preset->getUuid();
    if(auto bank = m_presetManager->findBankWithPreset(presetUuid))
    {
      auto scope = Application::get().getUndoScope()->startTransaction("Select Preset");
      bank->selectPreset(scope->getTransaction(), presetUuid);
    }
  }
}
void PresetManagerUseCases::moveLeft(Bank* bank)
{
  if(bank)
  {
    auto pos = m_presetManager->getBankPosition(bank->getUuid());
    if(pos > 0)
    {
      auto scope = bank->getUndoScope().startTransaction("Move bank '%0' left", bank->getName(true));
      m_presetManager->setOrderNumber(scope->getTransaction(), bank->getUuid(), pos - 1);
    }
  }
}

void PresetManagerUseCases::moveRight(Bank* bank)
{
  if(bank)
  {
    auto pos = m_presetManager->getBankPosition(bank->getUuid()) + 1;
    if(pos < m_presetManager->getNumBanks())
    {
      auto scope = bank->getUndoScope().startTransaction("Move bank '%0' right", bank->getName(true));
      m_presetManager->setOrderNumber(scope->getTransaction(), bank->getUuid(), pos);
    }
  }
}

void PresetManagerUseCases::stepBankSelection(int inc, bool shift)
{
  auto scope = m_presetManager->getUndoScope().startTransaction("Select Bank");

  if(shift && m_presetManager->getNumBanks() > 0)
  {
    if(inc < 0)
      m_presetManager->selectBank(scope->getTransaction(), m_presetManager->getBanks().front()->getUuid());
    else
      m_presetManager->selectBank(scope->getTransaction(), m_presetManager->getBanks().back()->getUuid());
  }
  else
  {
    while(inc < 0)
    {
      m_presetManager->selectPreviousBank(scope->getTransaction());
      inc++;
    }

    while(inc > 0)
    {
      m_presetManager->selectNextBank(scope->getTransaction());
      inc--;
    }
  }
}

void PresetManagerUseCases::sortBankNumbers()
{
  auto scope = m_presetManager->getUndoScope().startTransaction("Sort Bank Numbers");
  auto transaction = scope->getTransaction();

  ClusterEnforcement ce(m_presetManager);
  auto newBanks = ce.sortBanks();
  m_presetManager->sortBanks(transaction, newBanks);
}

void PresetManagerUseCases::dropPresets(const std::string& anchorUuid, PresetManagerUseCases::DropActions action,
                                        const Glib::ustring& csv)
{
  auto actionToOffset = [](DropActions action) {
    switch(action)
    {
      case DropActions::Above:
        return 0;
      case DropActions::Below:
        return 1;
      case DropActions::Onto:
        return 1;
    }
  };

  int offset = actionToOffset(action);

  std::vector<std::string> strs;
  boost::split(strs, csv, boost::is_any_of(","));

  if(auto anchor = m_presetManager->findPreset(anchorUuid))
  {
    auto scope = m_presetManager->getUndoScope().startTransaction("Drop Presets");
    auto transaction = scope->getTransaction();

    auto bank = static_cast<Bank*>(anchor->getParent());
    auto anchorPos = static_cast<int>(bank->getPresetPosition(anchorUuid));
    auto pos = std::max(anchorPos + offset, 0);

    for(const auto& presetUUID : strs)
    {
      if(auto src = m_presetManager->findPreset(presetUUID))
      {
        if(src->getParent() == anchor->getParent())
        {
          bank->movePreset(transaction, src, anchor);
        }
        else
        {
          bank->insertPreset(transaction, size_t(pos), std::make_unique<Preset>(bank, *src, true));
          pos++;
        }
      }
    }

    if(action == DropActions::Onto)
    {
      if(auto droppedOntoPreset = m_presetManager->findPreset(anchorUuid))
      {
        static_cast<Bank*>(droppedOntoPreset->getParent())->deletePreset(transaction, anchorUuid);
      }
    }
  }
}
