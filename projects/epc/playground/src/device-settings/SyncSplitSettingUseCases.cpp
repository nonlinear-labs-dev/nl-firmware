#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameter_declarations.h>
#include <http/UndoScope.h>
#include <parameters/SplitPointParameter.h>
#include "SyncSplitSettingUseCases.h"

SyncSplitSettingUseCases::SyncSplitSettingUseCases(SplitPointSyncParameters& setting, PresetManager& pm)
    : m_setting { setting }
    , m_presetManager { pm }
    , m_splitI { pm.getEditBuffer()->findAndCastParameterByID<SplitPointParameter>(
          { C15::PID::Split_Split_Point, VoiceGroup::I }) }
    , m_splitII { pm.getEditBuffer()->findAndCastParameterByID<SplitPointParameter>(
          { C15::PID::Split_Split_Point, VoiceGroup::II }) }
{
}

void SyncSplitSettingUseCases::enableSyncSetting(UNDO::Transaction* t)
{
  undoableSetSetting(t, true);
  auto cpI = m_splitI->getControlPositionValue();
  auto cpII = m_splitII->getValue().getNextStepValue(cpI, 1, false, false);
  m_splitII->updateCPFromSyncChange(t, cpII);

  //TODO implement rules to copy ModAspects from I into II or vice versa
  if(m_splitI->getModulationSource() != MacroControls::NONE)
  {
    m_splitII->setModulationAmount(t, m_splitI->getModulationAmount());
    m_splitII->setModulationSource(t, m_splitI->getModulationSource());
  }
  else if(m_splitII->getModulationSource() != MacroControls::NONE)
  {
    m_splitI->setModulationAmount(t, m_splitII->getModulationAmount());
    m_splitI->setModulationSource(t, m_splitII->getModulationSource());
  }
}

void SyncSplitSettingUseCases::disableSyncSetting(UNDO::Transaction* t)
{
  undoableSetSetting(t, false);
}

void SyncSplitSettingUseCases::undoableSetSetting(UNDO::Transaction* transaction, bool state)
{
  m_setting.undoableSet(transaction, state);
}

void SyncSplitSettingUseCases::enableSyncSetting()
{
  auto scope = m_presetManager.getUndoScope().startTransaction("Enable Split Sync");
  enableSyncSetting(scope->getTransaction());
}

void SyncSplitSettingUseCases::disableSyncSetting()
{
  auto scope = m_presetManager.getUndoScope().startTransaction("Disable Split Sync");
  disableSyncSetting(scope->getTransaction());
}

void SyncSplitSettingUseCases::updateFromWebUI(const Glib::ustring& value)
{
  if(value == "on")
  {
    enableSyncSetting();
  }
  else
  {
    disableSyncSetting();
  }
}

void SyncSplitSettingUseCases::toggleSyncSetting()
{
  if(m_setting.get())
    disableSyncSetting();
  else
    enableSyncSetting();
}
