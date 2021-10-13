#include "StoreUseCaseHelper.h"
#include <presets/Preset.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/SplitPointParameter.h>
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <parameter_declarations.h>
#include <device-settings/SyncSplitSettingUseCases.h>

void StoreUseCaseHelper::onStore(UNDO::Transaction* transaction, Preset& preset, PresetManager& pm, Settings& settings)
{
  if(preset.getType() == SoundType::Split)
  {
    StoreUseCaseHelper::updateSyncSettingOnPresetStore(transaction, pm, settings);
  }

  pm.onPresetStored();
}

void StoreUseCaseHelper::updateSyncSettingOnPresetStore(UNDO::Transaction* transaction, PresetManager& pm, Settings& settings)
{
  auto eb = pm.getEditBuffer();
  if(auto s = eb->findAndCastParameterByID<SplitPointParameter>({ C15::PID::Split_Split_Point, VoiceGroup::I }))
  {
    SyncSplitSettingUseCases useCase(*settings.getSetting<SplitPointSyncParameters>(), pm);

    if(s->hasOverlap())
    {
      useCase.disableSyncSetting(transaction);
    }
    else
    {
      useCase.enableSyncSetting(transaction);
    }
  }
}
