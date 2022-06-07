#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <utility>
#include "BankUseCases.h"
#include "presets/Bank.h"
#include "presets/PresetManager.h"
#include "presets/Preset.h"
#include "presets/EditBuffer.h"
#include "StoreUseCaseHelper.h"
#include "EditBufferStorePreparation.h"
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>
#include <Application.h>
#include <tools/FileSystem.h>
#include <tools/TimeTools.h>
#include <xml/FileOutStream.h>
#include <xml/VersionAttribute.h>

BankUseCases::BankUseCases(Bank* bank, Settings& settings)
    : m_bank { bank }
    , m_settings{ settings }
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

  if(auto pm = getPresetManager())
  {
    pm->getEditBuffer()->undoableUpdateLoadedPresetInfo(transaction);
  }
}

void BankUseCases::stepPresetSelection(int inc)
{
  if(auto presetManager = getPresetManager())
  {
    const bool directLoad = isDirectLoadActive();
    if(auto current = m_bank->getSelectedPreset())
    {
      int currentPos = static_cast<int>(m_bank->getPresetPosition(current));
      int numPresets = static_cast<int>(m_bank->getNumPresets());
      auto presetPosToSelect = std::max(0, std::min(numPresets - 1, currentPos + inc));
      if(auto selectedPreset = presetManager->getSelectedPreset())
      {
        if(auto presetToSelect = m_bank->getPresetAt(presetPosToSelect))
        {
          if(presetToSelect != selectedPreset)
          {
            Glib::ustring name {};
            if(directLoad)
              name = presetToSelect->buildUndoTransactionTitle("Select and Load Preset");
            else
              name = presetToSelect->buildUndoTransactionTitle("Select Preset");

            auto scope = m_bank->getUndoScope().startContinuousTransaction(m_bank, std::chrono::hours(1), name);
            m_bank->selectPreset(scope->getTransaction(), presetToSelect->getUuid());

            if(directLoad)
            {
              presetManager->getEditBuffer()->undoableLoad(scope->getTransaction(), presetToSelect, true);
            }
          }
        }
      }
    }
  }
}

void BankUseCases::dropPresets(const std::string& csv)
{
  if(auto pm = getPresetManager())
  {
    auto scope = m_bank->getUndoScope().startTransaction("Drop Presets on Bank");
    auto transaction = scope->getTransaction();

    std::vector<std::string> strs;
    boost::split(strs, csv, boost::is_any_of(","));

    for(const auto& uuid : strs)
    {
      if(auto src = pm->findPreset(Uuid { uuid }))
      {
        m_bank->appendPreset(transaction, std::make_unique<Preset>(m_bank, *src));

        if(m_bank == src->getParent())
          m_bank->deletePreset(transaction, Uuid { uuid });
      }
    }
  }
}

void BankUseCases::dropBank(const Bank* source)
{
  if(auto pm = getPresetManager())
  {
    if(m_bank != source && source)
    {
      auto insertPos = m_bank->getNumPresets();
      auto scope = pm->getUndoScope().startTransaction("Drop bank '%0' into bank '%1'", source->getName(true),
                                                       m_bank->getName(true));
      auto transaction = scope->getTransaction();
      size_t i = 0;

      source->forEachPreset([&](auto p) {
        m_bank->insertPreset(transaction, insertPos + i, std::make_unique<Preset>(m_bank, *p));
        i++;
      });
    }
  }
}

void BankUseCases::dropBankOnPreset(const Bank* sourceBank, const Uuid& presetAnchor)
{
  if(m_bank != sourceBank)
  {
    auto srcBankName = sourceBank->getName(true);
    auto tgtBankName = m_bank->getName(true);
    auto scope = m_bank->getUndoScope().startTransaction("Drop bank '%0' into bank '%1'", srcBankName, tgtBankName);
    auto transaction = scope->getTransaction();

    size_t insertPos = m_bank->getPresetPosition(presetAnchor) + 1;
    size_t numPresets = sourceBank->getNumPresets();

    for(size_t i = 0; i < numPresets; i++)
    {
      auto p = std::make_unique<Preset>(m_bank, *sourceBank->getPresetAt(i));
      m_bank->insertPreset(transaction, insertPos + i, std::move(p));
    }

    m_bank->deletePreset(scope->getTransaction(), presetAnchor);
  }
}

void BankUseCases::moveBank(const std::string& x, const std::string& y)
{
  if(m_bank->getX() != x || m_bank->getY() != y)
  {
    auto scope = m_bank->getUndoScope().startTransaction("Move preset bank '%0'", m_bank->getName(true));
    auto transaction = scope->getTransaction();
    m_bank->setX(transaction, x);
    m_bank->setY(transaction, y);
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

    if(auto pm = getPresetManager())
    {
      auto eb = pm->getEditBuffer();
      eb->undoableUpdateLoadedPresetInfo(scope->getTransaction());
    }
  }
}

void BankUseCases::selectPreset(int pos)
{
  auto pm = getPresetManager();
  if(pos < m_bank->getNumPresets())
  {
    if(auto presetToSelect = m_bank->getPresetAt(pos))
    {
      if(pm->getSelectedPreset() != presetToSelect)
      {
        const auto directLoad = isDirectLoadActive();
        Glib::ustring name {};
        if(directLoad)
          name = presetToSelect->buildUndoTransactionTitle("Select and Load Preset");
        else
          name = presetToSelect->buildUndoTransactionTitle("Select Preset");

        auto scope = m_bank->getUndoScope().startContinuousTransaction(m_bank, std::chrono::hours(1), name);
        m_bank->selectPreset(scope->getTransaction(), pos);

        if(isDirectLoadActive())
        {
          auto eb = pm->getEditBuffer();
          eb->undoableLoad(scope->getTransaction(), presetToSelect, true);
        }
      }
    }
  }
}

void BankUseCases::setAttribute(const Glib::ustring& key, const Glib::ustring& value)
{
  const auto oldValue = m_bank->getAttribute(key, "");
  if(oldValue != value)
  {
    auto scope = m_bank->getUndoScope().startTransaction("Set Bank attribute");
    auto transaction = scope->getTransaction();
    m_bank->setAttribute(transaction, key, value);
    m_bank->updateLastModifiedTimestamp(transaction);
  }
}

bool BankUseCases::isDirectLoadActive() const
{
  return m_settings.getSetting<DirectLoadSetting>()->get();
}

void BankUseCases::setCollapsed(bool b)
{
  if(m_bank->isCollapsed() != b)
  {
    std::string name = b ? "Collapse" : "Maximize";
    auto bankName = m_bank->getName(true);
    auto scope = m_bank->getUndoScope().startTransaction(name + " Bank " + bankName);
    auto transaction = scope->getTransaction();
    m_bank->setAttribute(transaction, "collapsed", b ? "true" : "false");
    m_bank->updateLastModifiedTimestamp(transaction);
  }
}

void BankUseCases::exportBankToFile(const std::string& outFile)
{
  GenericScopeGuard syncAfterAllFileOperation([] {}, FileSystem::syncAll);
  auto scope = UNDO::Scope::startTrashTransaction();
  m_bank->setAttribute(scope->getTransaction(), "Date of Export File", TimeTools::getAdjustedIso());
  m_bank->setAttribute(scope->getTransaction(), "Name of Export File", outFile);
  PresetBankSerializer serializer(m_bank, {}, false);
  FileOutStream stream(outFile, false);
  XmlWriter writer(stream);
  serializer.write(writer, VersionAttribute::get());
}

void BankUseCases::insertBank(Bank* toInsert, size_t insertPosition)
{
  auto pm = getPresetManager();
  auto scope = pm->getUndoScope().startTransaction("Drop bank '%0' into bank '%1'", toInsert->getName(true),
                                                   m_bank->getName(true));
  auto transaction = scope->getTransaction();

  toInsert->forEachPreset([&, i = 0](auto p) mutable {
    m_bank->insertPreset(transaction, insertPosition + (i++), std::make_unique<Preset>(m_bank, *p));
  });
}

Preset* BankUseCases::insertEditBufferAtPosition(int anchor)
{
  auto pm = getPresetManager();
  auto eb = pm->getEditBuffer();
  const auto ebWasModified = eb->isModified();
  EditBufferStorePreparation ebsp(*eb);

  auto scope = pm->getUndoScope().startTransaction("Insert Editbuffer into Bank '%0'", m_bank->getName(true));
  auto transaction = scope->getTransaction();
  auto preset = m_bank->insertPreset(transaction, anchor, std::make_unique<Preset>(m_bank, *eb));
  if(ebWasModified)
    preset->guessName(transaction);
  eb->undoableSetLoadedPresetInfo(transaction, preset);
  pm->selectBank(transaction, m_bank->getUuid());
  m_bank->selectPreset(transaction, preset->getUuid());
  return preset;
}

Preset* BankUseCases::appendEditBuffer()
{
  auto pm = getPresetManager();
  auto eb = pm->getEditBuffer();
  const auto ebWasModified = eb->isModified();
  EditBufferStorePreparation ebsp(*eb);

  auto scope = pm->getUndoScope().startTransaction("Append Editbuffer into Bank '%0'", m_bank->getName(true));
  auto transaction = scope->getTransaction();
  auto preset = m_bank->appendPreset(transaction, std::make_unique<Preset>(m_bank, *eb));
  if(ebWasModified)
    preset->guessName(transaction);
  eb->undoableSetLoadedPresetInfo(transaction, preset);
  pm->selectBank(transaction, m_bank->getUuid());
  m_bank->selectPreset(transaction, preset->getUuid());
  return preset;
}

Preset* BankUseCases::appendEditBufferAsPresetWithUUID(Uuid uuid)
{
  return insertEditBufferAsPresetWithUUID(m_bank->getNumPresets(), std::move(uuid));
}

Preset* BankUseCases::insertEditBufferAsPresetWithUUID(size_t pos, Uuid uuid)
{
  auto pm = getPresetManager();
  auto eb = pm->getEditBuffer();

  if(uuid.empty())
    uuid.generate();

  EditBufferStorePreparation ebsp(*eb);

  auto scope = pm->getUndoScope().startTransaction("Insert preset at position %0", pos + 1);
  auto transaction = scope->getTransaction();
  const auto ebIsModified = eb->isModified();

  auto preset = m_bank->insertPreset(transaction, pos, std::make_unique<Preset>(m_bank, *eb, uuid));

  eb->undoableSetLoadedPresetInfo(transaction, preset);
  pm->selectBank(transaction, m_bank->getUuid());
  m_bank->selectPreset(transaction, preset->getUuid());

  if(ebIsModified)
    preset->guessName(transaction);

  StoreUseCaseHelper::onStore(transaction, *preset, *pm, m_settings);

  assert(pm->getSelectedBank() == m_bank);
  assert(m_bank->getSelectedPreset() == preset);

  return preset;
}

PresetManager* BankUseCases::getPresetManager() const
{
  auto pm = m_bank->getPresetManager();
  nltools_assertOnDevPC(pm != nullptr);
  return pm;
}

void BankUseCases::selectFirstOrLastPreset(int inc)
{
  Preset* newPreset = nullptr;
  if(inc > 0)
  {
    newPreset = m_bank->getPresetAt(0);
  }
  else if(inc < 0)
  {
    newPreset = m_bank->getPresetAt(m_bank->getNumPresets() - 1);
  }

  if(newPreset)
  {
    auto name = newPreset->buildUndoTransactionTitle("Select Preset");
    auto scope = m_bank->getUndoScope().startTransaction(name);
    m_bank->selectPreset(scope->getTransaction(), newPreset->getUuid());
  }
}
