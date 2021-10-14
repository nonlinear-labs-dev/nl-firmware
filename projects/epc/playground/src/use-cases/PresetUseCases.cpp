#include "PresetUseCases.h"
#include "StoreUseCaseHelper.h"
#include <presets/Preset.h>
#include <libundo/undo/Scope.h>
#include <nltools/Assert.h>
#include <presets/Bank.h>
#include <presets/PresetManager.h>
#include <parameter_declarations.h>
#include <presets/EditBuffer.h>
#include <parameters/SplitPointParameter.h>
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <device-settings/SyncSplitSettingUseCases.h>

PresetUseCases::PresetUseCases(Preset* p, Settings& settings)
    : m_preset { p }
    , m_settings{ settings }
{
  nltools_assertAlways(m_preset != nullptr);
}

void PresetUseCases::rename(const std::string& newName)
{
  if(m_preset->getName() != newName)
  {
    auto scope = m_preset->getUndoScope().startTransaction("Rename preset");
    m_preset->setName(scope->getTransaction(), newName);
  }
}

void PresetUseCases::setComment(const Glib::ustring& comment)
{
  if(m_preset->getComment() != comment)
  {
    auto scope = m_preset->getUndoScope().startTransaction("Set Preset Comment");
    m_preset->setAttribute(scope->getTransaction(), "Comment", comment);
  }
}

void PresetUseCases::setAttribute(const Glib::ustring& key, const Glib::ustring& value)
{
  auto scope = m_preset->getUndoScope().startTransaction("Set Preset attribute");
  auto transaction = scope->getTransaction();
  m_preset->setAttribute(transaction, key, value);
  m_preset->setAutoGeneratedAttributes(transaction);
}

void PresetUseCases::overwriteWithEditBuffer(EditBuffer& editBuffer)
{
  auto scope = m_preset->getUndoScope().startTransaction("Overwrite '%0' with Editbuffer", m_preset->getName());
  m_preset->copyFrom(scope->getTransaction(), &editBuffer);
}

void PresetUseCases::overwriteWithPreset(Preset* source)
{
  auto scope = m_preset->getUndoScope().startTransaction("Overwrite preset '%0' with '%1'", m_preset->getName(),source->getName());
  if(auto targetBank = dynamic_cast<Bank*>(m_preset->getParent()))
  {
    if(auto presetManager = targetBank->getPresetManager())
    {
      auto transaction = scope->getTransaction();
      m_preset->copyFrom(transaction, source);
      targetBank->selectPreset(transaction, m_preset->getUuid());
      presetManager->selectBank(transaction, targetBank->getUuid());
      StoreUseCaseHelper::onStore(transaction, *m_preset, *presetManager, m_settings);
      assert(presetManager->getSelectedBank() == targetBank);
      assert(targetBank->getSelectedPreset() == m_preset);
    }
  }
}