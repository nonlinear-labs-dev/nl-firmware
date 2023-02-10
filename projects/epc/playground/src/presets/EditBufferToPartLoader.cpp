#include "EditBufferToPartLoader.h"
#include "SendEditBufferScopeGuard.h"
#include "parameters/ScopedLock.h"
#include "parameters/scale-converters/ParabolicGainDbScaleConverter.h"
#include "parameters/ModulateableParameter.h"
#include <presets/EditBuffer.h>
#include <presets/Preset.h>
#include <parameters/Parameter.h>
#include <presets/PresetParameter.h>

namespace
{
  double linearLevelAttenuation(const double controlPosition, const double fadePosition)
  {
    const auto inverse = 1.0 - fadePosition;
    return controlPosition * inverse;
  }
}

void EditBufferToPartLoader::undoableLoadPresetPartIntoPart(UNDO::Transaction *transaction, const Preset *preset,
                                                            VoiceGroup from, VoiceGroup copyTo)
{
  if(!preset)
    return;

  SendEditBufferScopeGuard scopeGuard(transaction, true);

  switch(m_editBuffer.getType())
  {
    case SoundType::Single:
    case SoundType::Invalid:
      nltools::Log::error("can't load to part if current sound is single sound!");
      break;
    case SoundType::Split:
      if(preset->isDual())
        undoableLoadPresetPartIntoSplitSound(transaction, preset, from, copyTo);
      else
        loadSinglePresetIntoSplitPart(transaction, preset, from, copyTo);
      break;
    case SoundType::Layer:
      if(preset->isDual())
        undoableLoadPresetPartIntoLayerSound(transaction, preset, from, copyTo);
      else
        loadSinglePresetIntoLayerPart(transaction, preset, from, copyTo);
      break;
  }
}

void EditBufferToPartLoader::undoableLoadPresetPartIntoSplitSound(UNDO::Transaction *transaction, const Preset *preset,
                                                                  VoiceGroup from, VoiceGroup copyTo)
{
  SendEditBufferScopeGuard scopeGuard(transaction, true);
  m_editBuffer.setVoiceGroupName(transaction, preset->getName(), copyTo);

  {
    ScopedLock locks(transaction);
    locks.addLock({ C15::PID::FB_Mix_FX_Src, copyTo });
    locks.addLock({ C15::PID::Out_Mix_To_FX, copyTo });
    locks.addLock({ C15::PID::FB_Mix_FX, copyTo });

    for(auto p : getCrossFBParameters(copyTo))
      locks.addLock(p);

    for(auto vg : { VoiceGroup::I, VoiceGroup::II })
      locks.addLock(m_editBuffer.findParameterByID({ C15::PID::Split_Split_Point, vg }));

    m_editBuffer.copyFromSuper(transaction, preset, from, copyTo);
  }

  loadPartFromDualIntoSplit(transaction, from, copyTo, preset);
  copySumOfMasterGroupToVoiceGroupMasterGroup(transaction, preset, from, copyTo);

  initFadeParameters(transaction, copyTo);
  if(preset->getType() == SoundType::Layer)
  {
    initFadeParameters(transaction, invert(copyTo));

    const auto unisonTo = GroupId { "Unison", copyTo };
    const auto unisonI = GroupId { "Unison", VoiceGroup::I };
    const auto monoTo = GroupId { "Mono", copyTo };
    const auto monoI = GroupId { "Mono", VoiceGroup::I };

    m_editBuffer.getParameterGroupByID(unisonTo)->copyFrom(transaction, preset->findParameterGroup(unisonI));
    m_editBuffer.getParameterGroupByID(monoTo)->copyFrom(transaction, preset->findParameterGroup(monoI));
  }

  m_editBuffer.setVoiceGroupName(transaction, preset->getVoiceGroupName(from), copyTo);
}

void EditBufferToPartLoader::undoableLoadPresetPartIntoLayerSound(UNDO::Transaction *transaction, const Preset *preset,
                                                                  VoiceGroup copyFrom, VoiceGroup copyTo)
{
  SendEditBufferScopeGuard scopeGuard(transaction, true);
  m_editBuffer.setVoiceGroupName(transaction, preset->getName(), copyTo);

  {
    ScopedLock locks(transaction);
    locks.addLock({ C15::PID::FB_Mix_FX_Src, copyTo });
    locks.addLock({ C15::PID::Out_Mix_To_FX, copyTo });
    locks.addLock({ C15::PID::FB_Mix_FX, copyTo });
    locks.addLock({ C15::PID::Voice_Grp_Fade_From, copyTo });
    locks.addLock({ C15::PID::Voice_Grp_Fade_Range, copyTo });
    locks.addLock({ C15::PID::FB_Mix_Osc, copyTo });
    locks.addLock({ C15::PID::FB_Mix_Comb, copyTo });
    locks.addLock({ C15::PID::FB_Mix_Comb_Src, copyTo });
    locks.addLock({ C15::PID::FB_Mix_SVF, copyTo });
    locks.addLock({ C15::PID::FB_Mix_SVF_Src, copyTo });

    if(preset->getType() == SoundType::Split)
    {
      locks.addLocks(getCrossFBParameters(copyTo));
    }

    if(preset->isDual())
    {
      locks.addGroupLock({ "Mono", VoiceGroup::I });
      locks.addGroupLock({ "Mono", VoiceGroup::II });
      locks.addGroupLock({ "Unison", VoiceGroup::I });
      locks.addGroupLock({ "Unison", VoiceGroup::II });
    }

    m_editBuffer.copyFromSuper(transaction, preset, copyFrom, copyTo);

    if(preset->isDual())
      copySumOfMasterGroupToVoiceGroupMasterGroup(transaction, preset, copyFrom, copyTo);
    else
      copySinglePresetMasterToPartMaster(transaction, preset, copyTo);

    initFadeParameters(transaction, copyTo);
  }

  m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, copyTo })->setCPFromHwui(transaction, 0);

  loadPartFromDualIntoLayer(transaction, copyFrom, copyTo, preset);

  m_editBuffer.getParameterGroupByID({ "Unison", VoiceGroup::II })
      ->undoableLoadDefault(transaction, Defaults::FactoryDefault);
  m_editBuffer.getParameterGroupByID({ "Mono", VoiceGroup::II })
      ->undoableLoadDefault(transaction, Defaults::FactoryDefault);

  if(preset->isDual())
    m_editBuffer.setVoiceGroupName(transaction, preset->getVoiceGroupName(copyFrom), copyTo);
  else
    m_editBuffer.setVoiceGroupName(transaction, preset->getName(), copyTo);
}

void EditBufferToPartLoader::loadSinglePresetIntoSplitPart(UNDO::Transaction *transaction, const Preset *preset,
                                                           VoiceGroup from, VoiceGroup loadInto)
{
  {
    auto toFxParam = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, loadInto });
    auto splitPointI = m_editBuffer.findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I });
    auto splitPointII = m_editBuffer.findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II });

    auto monophonicParameters
        = m_editBuffer.findAllParametersOfType(C15::Descriptors::ParameterType::Monophonic_Unmodulateable);
    auto monophonicModparams
        = m_editBuffer.findAllParametersOfType(C15::Descriptors::ParameterType::Monophonic_Modulateable);

    auto monophonicsNotInLoadInto = combine(monophonicModparams, monophonicParameters);
    monophonicsNotInLoadInto.erase(
        std::remove_if(monophonicsNotInLoadInto.begin(), monophonicsNotInLoadInto.end(),
                       [loadInto](const auto &id) { return id.getVoiceGroup() == loadInto; }),
        monophonicsNotInLoadInto.end());

    ScopedLock locks(transaction);
    locks.addLock(toFxParam);
    locks.addLock(splitPointI);
    locks.addLock(splitPointII);
    locks.addLocks(getFeedbackParametersLockedForLoadToPart(loadInto));
    locks.addLocks(monophonicsNotInLoadInto);

    m_editBuffer.copyFromSuper(transaction, preset, from, loadInto);
  }

  copyPolyParametersFromI(transaction, preset, loadInto);

  copySinglePresetMasterToPartMaster(transaction, preset, loadInto);

  initFadeFrom(transaction, loadInto);
  loadPartFromSingleIntoDual(transaction, from, loadInto, preset);
  m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, loadInto })->setCPFromHwui(transaction, 0);

  updateLoadFromPartOrigin(transaction, preset, from, loadInto);
  m_editBuffer.setVoiceGroupName(transaction, preset->getName(), loadInto);
}

void EditBufferToPartLoader::loadSinglePresetIntoLayerPart(UNDO::Transaction *transaction, const Preset *preset,
                                                           VoiceGroup from, VoiceGroup loadTo)
{
  auto toFxParam = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, loadTo });
  auto fadeFromParams = { m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Fade_From, loadTo }),
                          m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Fade_Range, loadTo }) };

  {
    auto allNonLoadIntoMonophonics
        = m_editBuffer.findAllParametersOfType({ C15::Descriptors::ParameterType::Monophonic_Unmodulateable,
                                                 C15::Descriptors::ParameterType::Monophonic_Modulateable });
    allNonLoadIntoMonophonics.erase(std::remove_if(allNonLoadIntoMonophonics.begin(), allNonLoadIntoMonophonics.end(),
                                                   [loadTo](const auto &id) { return id.getVoiceGroup() == loadTo; }),
                                    allNonLoadIntoMonophonics.end());

    ScopedLock locks(transaction);
    locks.addLock(toFxParam);
    locks.addGroupLock({ "Unison", VoiceGroup::I });
    locks.addGroupLock({ "Unison", VoiceGroup::II });
    locks.addGroupLock({ "Mono", VoiceGroup::I });
    locks.addGroupLock({ "Mono", VoiceGroup::II });
    locks.addLocks(getFeedbackParametersLockedForLoadToPart(loadTo));
    locks.addLocks(allNonLoadIntoMonophonics);

    for(auto p : fadeFromParams)
      locks.addLock(p);

    m_editBuffer.copyFromSuper(transaction, preset, from, loadTo);
  }

  copyPolyParametersFromI(transaction, preset, loadTo);

  copySpecialToFXParamForLoadSingleIntoDualPart(transaction, from, loadTo, preset);

  copySinglePresetMasterToPartMaster(transaction, preset, loadTo);

  m_editBuffer.getParameterGroupByID({ "Unison", VoiceGroup::II })
      ->undoableLoadDefault(transaction, Defaults::FactoryDefault);
  m_editBuffer.getParameterGroupByID({ "Mono", VoiceGroup::II })
      ->undoableLoadDefault(transaction, Defaults::FactoryDefault);

  loadPartFromSingleIntoDual(transaction, from, loadTo, preset);
  m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, loadTo })->setCPFromHwui(transaction, 0);

  updateLoadFromPartOrigin(transaction, preset, from, loadTo);

  m_editBuffer.setVoiceGroupName(transaction, preset->getName(), loadTo);
}

void EditBufferToPartLoader::copySinglePresetMasterToPartMaster(UNDO::Transaction *transaction, const Preset *preset,
                                                                VoiceGroup targetGroup)
{
  auto presetGlobalVolume = preset->findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global }, false);
  auto presetGlobalTune = preset->findParameterByID({ C15::PID::Master_Tune, VoiceGroup::Global }, false);

  auto ebGlobalVolume = m_editBuffer.findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
  auto ebGlobalTune = m_editBuffer.findParameterByID({ C15::PID::Master_Tune, VoiceGroup::Global });

  auto partVolume
      = m_editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Voice_Grp_Volume, targetGroup });
  auto partTune
      = m_editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Voice_Grp_Tune, targetGroup });

  if(presetGlobalTune && presetGlobalVolume)
  {
    partTune->setCPFromHwui(transaction, presetGlobalTune->getValue() - ebGlobalTune->getControlPositionValue());
    partTune->setModulationSource(transaction, presetGlobalTune->getModulationSource());
    partTune->setModulationAmount(transaction, presetGlobalTune->getModulationAmount());

    ParabolicGainDbScaleConverter volumeConverter;
    auto presetVolumeDisplay = volumeConverter.controlPositionToDisplay(presetGlobalVolume->getValue());
    auto ebVolumeDisplay = volumeConverter.controlPositionToDisplay(ebGlobalVolume->getControlPositionValue());
    auto newVolumeCP = volumeConverter.displayToControlPosition(presetVolumeDisplay - ebVolumeDisplay);

    partVolume->setCPFromHwui(transaction, newVolumeCP);
    partVolume->setModulationSource(transaction, presetGlobalVolume->getModulationSource());
    partVolume->setModulationAmount(transaction, presetGlobalVolume->getModulationAmount());
  }
  else
  {
    partTune->loadDefault(transaction, Defaults::FactoryDefault);
    partVolume->loadDefault(transaction, Defaults::FactoryDefault);
  }
}

void EditBufferToPartLoader::loadPartFromDualIntoSplit(UNDO::Transaction *transaction, VoiceGroup loadFrom,
                                                       VoiceGroup loadTo, const Preset *preset)
{
  auto source_m_fx = preset->findParameterByID({ C15::PID::FB_Mix_FX, loadFrom }, false)->getValue();
  auto source_m_fx_src = preset->findParameterByID({ C15::PID::FB_Mix_FX_Src, loadFrom }, false)->getValue();

  // crucial: Comb/SVF/FX channel load with attenuation
  auto new_fx = parabolicLevelAttenuation(source_m_fx, source_m_fx_src);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX, loadTo })->setCPFromHwui(transaction, new_fx);
  // crucial: defaults
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc_Src, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Comb_Src, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, loadTo })->setCPFromHwui(transaction, 0);

  //copy from source
  auto fb_comb_preset = preset->findParameterByID({ C15::PID::FB_Mix_Comb, loadFrom }, false);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Comb, loadTo })
      ->setCPFromHwui(transaction, fb_comb_preset->getValue());

  auto fb_svf_preset = preset->findParameterByID({ C15::PID::FB_Mix_SVF, loadFrom }, false);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_SVF, loadTo })
      ->setCPFromHwui(transaction, fb_svf_preset->getValue());

  loadPartFromDualIntoDual_OutputMixer(transaction, loadFrom, loadTo, preset);
}

void EditBufferToPartLoader::copySumOfMasterGroupToVoiceGroupMasterGroup(UNDO::Transaction *transaction,
                                                                         const Preset *preset, VoiceGroup copyFrom,
                                                                         VoiceGroup copyTo)
{
  auto presetGlobalVolume = preset->findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global }, false);
  auto presetGlobalTune = preset->findParameterByID({ C15::PID::Master_Tune, VoiceGroup::Global }, false);

  auto presetPartVolume = preset->findParameterByID({ C15::PID::Voice_Grp_Volume, copyFrom }, false);
  auto presetPartTune = preset->findParameterByID({ C15::PID::Voice_Grp_Tune, copyFrom }, false);

  auto globalVolume = m_editBuffer.findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
  auto globalTune = m_editBuffer.findParameterByID({ C15::PID::Master_Tune, VoiceGroup::Global });

  auto partVolume
      = m_editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Voice_Grp_Volume, copyTo });
  auto partTune = m_editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Voice_Grp_Tune, copyTo });

  if(presetGlobalVolume && presetGlobalTune && partVolume && partTune)
  {
    auto volumeScaleConverter
        = dynamic_cast<const ParabolicGainDbScaleConverter *>(partVolume->getValue().getScaleConverter());

    auto ebGlobalVolumeDV = volumeScaleConverter->controlPositionToDisplay(globalVolume->getControlPositionValue());
    auto presetGlobalVolumeDV = volumeScaleConverter->controlPositionToDisplay(presetGlobalVolume->getValue());
    auto presetPartVolumeDV = volumeScaleConverter->controlPositionToDisplay(
        presetPartVolume ? presetPartVolume->getValue() : partVolume->getDefaultValue());

    auto newVolumeDV = (presetGlobalVolumeDV + presetPartVolumeDV) - ebGlobalVolumeDV;
    auto newVolumeCP = volumeScaleConverter->displayToControlPosition(newVolumeDV);
    partVolume->setCPFromHwui(transaction, newVolumeCP);

    auto ebGlobalTuneCP = globalTune->getControlPositionValue();

    partTune->setCPFromHwui(
        transaction,
        (presetGlobalTune->getValue() + (presetPartTune ? presetPartTune->getValue() : partTune->getDefaultValue()))
            - ebGlobalTuneCP);

    if(preset->getType() == SoundType::Single)
    {
      partTune->setModulationAmount(transaction, presetGlobalTune->getModulationAmount());
      partTune->setModulationSource(transaction, presetGlobalTune->getModulationSource());

      partVolume->setModulationSource(transaction, presetGlobalVolume->getModulationSource());
      partVolume->setModulationAmount(transaction, presetGlobalVolume->getModulationAmount());
    }
  }
  else if(partTune && partVolume)
  {
    partTune->loadDefault(transaction, Defaults::FactoryDefault);
    partVolume->loadDefault(transaction, Defaults::FactoryDefault);
  }
}

void EditBufferToPartLoader::initFadeParameters(UNDO::Transaction *transaction, VoiceGroup group)
{
  auto f1 = m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Fade_From, group });
  if(!f1->isLocked())
    f1->loadDefault(transaction, Defaults::FactoryDefault);

  auto f2 = m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Fade_Range, group });
  if(!f2->isLocked())
    f2->loadDefault(transaction, Defaults::FactoryDefault);
}

void EditBufferToPartLoader::loadPartFromDualIntoLayer(UNDO::Transaction *transaction, VoiceGroup loadFrom,
                                                       VoiceGroup loadTo, const Preset *preset)
{
  auto source_m_comb = preset->findParameterByID({ C15::PID::FB_Mix_Comb, loadFrom }, false)->getValue();
  auto source_m_comb_src = preset->findParameterByID({ C15::PID::FB_Mix_Comb_Src, loadFrom }, false)->getValue();
  auto source_m_svf = preset->findParameterByID({ C15::PID::FB_Mix_SVF, loadFrom }, false)->getValue();
  auto source_m_svf_src = preset->findParameterByID({ C15::PID::FB_Mix_SVF_Src, loadFrom }, false)->getValue();
  auto source_m_fx = preset->findParameterByID({ C15::PID::FB_Mix_FX, loadFrom }, false)->getValue();
  auto source_m_fx_src = preset->findParameterByID({ C15::PID::FB_Mix_FX_Src, loadFrom }, false)->getValue();

  // crucial: Comb/SVF/FX channel load with attenuation
  auto new_comb = linearLevelAttenuation(source_m_comb, source_m_comb_src);
  auto new_svf = linearLevelAttenuation(source_m_svf, source_m_svf_src);
  auto new_fx = parabolicLevelAttenuation(source_m_fx, source_m_fx_src);

  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Comb, loadTo })->setCPFromHwui(transaction, new_comb);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_SVF, loadTo })->setCPFromHwui(transaction, new_svf);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX, loadTo })->setCPFromHwui(transaction, new_fx);

  // crucial: defaults
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc_Src, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Comb_Src, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_SVF_Src, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, loadTo })->setCPFromHwui(transaction, 0);

  loadPartFromDualIntoDual_OutputMixer(transaction, loadFrom, loadTo, preset);
}

std::vector<ParameterId> EditBufferToPartLoader::getFeedbackParametersLockedForLoadToPart(VoiceGroup loadTo)
{
  return { { C15::PID::FB_Mix_Osc, loadTo },      { C15::PID::FB_Mix_Osc_Src, loadTo },
           { C15::PID::FB_Mix_Comb_Src, loadTo }, { C15::PID::FB_Mix_SVF_Src, loadTo },
           { C15::PID::FB_Mix_FX, loadTo },       { C15::PID::FB_Mix_FX_Src, loadTo } };
}

void EditBufferToPartLoader::copyPolyParametersFromI(UNDO::Transaction *transaction, const Preset *preset,
                                                     VoiceGroup group)
{
  ScopedLock lock(transaction);
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc, group }));
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc_Src, group }));
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Comb_Src, group }));
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::FB_Mix_SVF_Src, group }));
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::FB_Mix_SVF, group }));
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX, group }));
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, group }));

  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, group }));

  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Fade_From, group }));
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Fade_Range, group }));

  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I }));
  lock.addLock(m_editBuffer.findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II }));

  lock.addGroupLock({ "Mono", VoiceGroup::I });
  lock.addGroupLock({ "Mono", VoiceGroup::II });

  lock.addGroupLock({ "Unison", VoiceGroup::I });
  lock.addGroupLock({ "Unison", VoiceGroup::II });

  for(auto targetGroup : m_editBuffer.getParameterGroups(group))
  {
    if(targetGroup->isPolyphonic())
    {
      targetGroup->copyFrom(transaction, preset->findParameterGroup({ targetGroup->getID().getName(), VoiceGroup::I }));
    }
  }
}

void EditBufferToPartLoader::initFadeFrom(UNDO::Transaction *transaction, VoiceGroup vg)
{
  m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Fade_From, vg })
      ->loadDefault(transaction, Defaults::FactoryDefault);
  m_editBuffer.findParameterByID({ C15::PID::Voice_Grp_Fade_Range, vg })
      ->loadDefault(transaction, Defaults::FactoryDefault);
}

void EditBufferToPartLoader::loadPartFromSingleIntoDual(UNDO::Transaction *transaction, VoiceGroup loadFrom,
                                                        VoiceGroup loadTo, const Preset *preset)
{
  auto source_m_fx_src = preset->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I }, false)->getValue();
  auto source_m_fx = preset->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I }, false)->getValue();

  const auto fade = loadFrom == VoiceGroup::I ? source_m_fx_src : 1.0 - source_m_fx_src;
  auto new_fx_pos = parabolicLevelAttenuation(source_m_fx, fade);

  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX, loadTo })->setCPFromHwui(transaction, new_fx_pos);

  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc_Src, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Comb_Src, loadTo })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, loadTo })->setCPFromHwui(transaction, 0);

  //copy from source
  auto preset_fb_svf = preset->findParameterByID({ C15::PID::FB_Mix_SVF, VoiceGroup::I }, false);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_SVF, loadTo })
      ->setCPFromHwui(transaction, preset_fb_svf->getValue());

  auto preset_fb_comb = preset->findParameterByID({ C15::PID::FB_Mix_Comb, VoiceGroup::I }, false);
  m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Comb, loadTo })
      ->setCPFromHwui(transaction, preset_fb_comb->getValue());

  //Output-Mixer:
  auto source_to_fx = preset->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I }, false);
  auto source_level = preset->findParameterByID({ C15::PID::Out_Mix_Lvl, VoiceGroup::I }, false);
  const auto send = loadFrom == VoiceGroup::I ? source_to_fx->getValue() : 1.0 - source_to_fx->getValue();

  auto newLevel = parabolicLevelAttenuation(source_level->getValue(), send);
  m_editBuffer.findParameterByID({ C15::PID::Out_Mix_Lvl, loadTo })->setCPFromHwui(transaction, newLevel);
}

void EditBufferToPartLoader::loadPartFromDualIntoDual_OutputMixer(UNDO::Transaction *transaction, VoiceGroup loadFrom,
                                                                  VoiceGroup loadTo, const Preset *preset)
{
  auto src_level = preset->findParameterByID({ C15::PID::Out_Mix_Lvl, loadFrom }, false);
  auto src_to_fx = preset->findParameterByID({ C15::PID::Out_Mix_To_FX, loadFrom }, false);
  auto new_level = parabolicLevelAttenuation(src_level->getValue(), src_to_fx->getValue());
  m_editBuffer.findParameterByID({ C15::PID::Out_Mix_Lvl, loadTo })->setCPFromHwui(transaction, new_level);
}

double EditBufferToPartLoader::parabolicLevelAttenuation(const double controlPosition, const double fadePosition)
{
  const auto inverse = 1.0 - fadePosition;
  return controlPosition * std::sqrt(inverse);
}

void EditBufferToPartLoader::updateLoadFromPartOrigin(UNDO::Transaction *transaction, const Preset *preset,
                                                      const VoiceGroup &from, const VoiceGroup &loadTo)
{
  if(loadTo == VoiceGroup::I)
  {
    m_editBuffer.setAttribute(transaction, "origin-I", preset->getUuid().raw());
    m_editBuffer.setAttribute(transaction, "origin-I-vg", toString(from));
  }
  else if(loadTo == VoiceGroup::II)
  {
    m_editBuffer.setAttribute(transaction, "origin-II", preset->getUuid().raw());
    m_editBuffer.setAttribute(transaction, "origin-II-vg", toString(from));
  }
}

void EditBufferToPartLoader::copySpecialToFXParamForLoadSingleIntoDualPart(UNDO::Transaction *transaction,
                                                                           VoiceGroup from, VoiceGroup to,
                                                                           const Preset *preset)
{
  auto out_mix_to_fx_I = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  auto out_mix_to_fx_II = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });

  auto target = to == VoiceGroup::I ? out_mix_to_fx_I : out_mix_to_fx_II;
  auto sourceParameter = preset->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I }, true);

  target->copyFrom(transaction, sourceParameter);

  if(from != VoiceGroup::I)
  {
    target->setCPFromHwui(transaction, 1.0 - sourceParameter->getValue());
  }
}
