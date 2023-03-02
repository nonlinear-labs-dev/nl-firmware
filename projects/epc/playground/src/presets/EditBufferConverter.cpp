#include "EditBufferConverter.h"
#include "SendEditBufferScopeGuard.h"
#include "parameters/ScopedLockByParameterTypes.h"
#include "parameters/ScopedLock.h"
#include "parameters/ModulateableParameter.h"
#include <presets/EditBuffer.h>
#include <parameters/Parameter.h>

void EditBufferConverter::convertToDual(UNDO::Transaction *transaction, SoundType newType, VoiceGroup currentVG)
{
  const auto oldType = m_editBuffer.m_type;

  if(oldType == newType)
    return;

  SendEditBufferScopeGuard scopeGuard(transaction, true);

  m_editBuffer.undoableSetTypeFromConvert(transaction, newType);

  if(oldType == SoundType::Single && newType == SoundType::Layer)
    undoableConvertSingleToLayer(transaction, currentVG);
  else if(oldType == SoundType::Single && newType == SoundType::Split)
    undoableConvertSingleToSplit(transaction, currentVG);
  else if(oldType == SoundType::Layer && newType == SoundType::Split)
    undoableConvertLayerToSplit(transaction);
  else if(oldType == SoundType::Split && newType == SoundType::Layer)
    undoableConvertSplitToLayer(transaction, currentVG);

  undoableUnmuteLayers(transaction);
  initRecallSetUUIDAndDoDeferredJobs(transaction);
}

void EditBufferConverter::undoableConvertDualToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  const auto oldType = m_editBuffer.getType();

  if(oldType == SoundType::Single)
    return;

  SendEditBufferScopeGuard scopeGuard(transaction, true);

  m_editBuffer.setName(transaction, m_editBuffer.getVoiceGroupName(copyFrom));
  m_editBuffer.undoableSetTypeFromConvert(transaction, SoundType::Single);

  copyPartVolumesToGlobalMasterAndFXMixForConvertDualToSingle(transaction, copyFrom);

  if(oldType == SoundType::Split)
    undoableConvertSplitToSingle(transaction, copyFrom);
  else if(oldType == SoundType::Layer)
    undoableConvertLayerToSingle(transaction, copyFrom);

  copyToFXAndFxFrom(transaction, copyFrom);
  initBothFadeFrom(transaction);
  initCrossFBExceptFromFX(transaction);
  initSplitPoint(transaction);

  {
    ScopedMonophonicParameterLock lock(transaction, m_editBuffer);
    m_editBuffer.forEachParameter(VoiceGroup::II,
                                  [&](Parameter *p) { p->loadDefault(transaction, Defaults::FactoryDefault); });
  }

  initVoiceGroupMasterAndTune(transaction, VoiceGroup::I);

  setVoiceGroupNames(transaction, "");
  initRecallSetUUIDAndDoDeferredJobs(transaction);
}

void EditBufferConverter::undoableConvertSingleToLayer(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  setVoiceGroupNames(transaction, m_editBuffer.getName());
  undoableInvertToFXII(transaction);

  {
    ScopedLockByParameterTypes effectsLock(transaction, getMonophonicTypes(), m_editBuffer);
    ScopedLock toFXLock(transaction, getParametersToLock(SoundType::Single));

    m_editBuffer.copyVoiceGroup(transaction, VoiceGroup::I, VoiceGroup::II);
  }

  copyGlobalMasterAndFXMixToPartVolumesForConvertSingleToDual(transaction);
  copyPartTuneFromMasterTuneAndDefaultMasterGroup(transaction);

  initBothFadeFrom(transaction);
  undoableUnisonMonoLoadDefaults(transaction, VoiceGroup::II);
  initSplitPoint(transaction);
  initCrossFBExceptFromFX(transaction);
  applyConversionRuleForFBMixFXFromSingleToDual(transaction);
}

void EditBufferConverter::undoableConvertSingleToSplit(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  setVoiceGroupNames(transaction, m_editBuffer.getName());
  undoableInvertToFXII(transaction);

  {
    ScopedLockByParameterTypes effectsLock(transaction, getMonophonicTypes(), m_editBuffer);
    ScopedLock toFXLock(transaction, getParametersToLock(SoundType::Single));

    m_editBuffer.copyVoiceGroup(transaction, VoiceGroup::I, VoiceGroup::II);
  }

  copyGlobalMasterAndFXMixToPartVolumesForConvertSingleToDual(transaction);
  copyPartTuneFromMasterTuneAndDefaultMasterGroup(transaction);
  initBothFadeFrom(transaction);
  initSplitPoint(transaction);
  initCrossFBExceptFromFX(transaction);
  applyConversionRuleForFBMixFXFromSingleToDual(transaction);
}

void EditBufferConverter::undoableConvertLayerToSplit(UNDO::Transaction *transaction)
{
  m_editBuffer.copyVoicesGroups(transaction, VoiceGroup::I, VoiceGroup::II);
  calculateSplitPointFromFadeParams(transaction);
  initBothFadeFrom(transaction);
  initCrossFBExceptFromFX(transaction);
}

void EditBufferConverter::undoableConvertSplitToLayer(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  m_editBuffer.copyVoicesGroups(transaction, copyFrom, invert(copyFrom));
  defaultFadeParameters(transaction);
  undoableUnisonMonoLoadDefaults(transaction, VoiceGroup::II);
  initSplitPoint(transaction);
  initCrossFBExceptFromFX(transaction);
}

void EditBufferConverter::undoableInvertToFXII(UNDO::Transaction *transaction) const
{
  auto toFXI = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  auto toFXII = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
  toFXII->setCPFromHwui(transaction, 1 - toFXI->getControlPositionValue());
}

void EditBufferConverter::undoableUnmuteLayers(UNDO::Transaction *transaction)
{
  m_editBuffer.findParameterByID({ C15::PID::Part_Mute, VoiceGroup::I })->setCPFromHwui(transaction, 0);
  m_editBuffer.findParameterByID({ C15::PID::Part_Mute, VoiceGroup::II })->setCPFromHwui(transaction, 0);
}

std::vector<Parameter *> EditBufferConverter::getParametersToLock(SoundType newType) const
{
  switch(newType)
  {
    case SoundType::Single:
      return { m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I }),
               m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II }) };
    default:
      return {};
  }
}

std::vector<C15::Descriptors::ParameterType> EditBufferConverter::getMonophonicTypes() const
{
  return { C15::Descriptors::ParameterType::Monophonic_Unmodulateable,
           C15::Descriptors::ParameterType::Monophonic_Modulateable };
}

namespace VolumeConversionRules
{
  double parabolicFadeCpToAmplitude(const double cp)
  {
    return 1.0 - (cp * cp);
  }

  double amplitudeToParabolicGainCp(const double af)
  {
    return std::sqrt(af) * 0.5;
  }

  double parabolicGainCpToAmplitude(const double cp)
  {
    return 4.0 * cp * cp;
  }

  double getPartVolumeForConvertSingleToDual(const double masterVolumeCP, const double fxMixCp, VoiceGroup vg)
  {
    const auto masterVolumeAmplitude = parabolicGainCpToAmplitude(masterVolumeCP);
    if(vg == VoiceGroup::I)
    {
      return amplitudeToParabolicGainCp(masterVolumeAmplitude * parabolicFadeCpToAmplitude(fxMixCp));
    }
    else if(vg == VoiceGroup::II)
    {
      return amplitudeToParabolicGainCp(masterVolumeAmplitude * parabolicFadeCpToAmplitude(1.0 - fxMixCp));
    }
    return 0;
  }
}

void EditBufferConverter::copyGlobalMasterAndFXMixToPartVolumesForConvertSingleToDual(UNDO::Transaction *transaction)
{
  auto master = m_editBuffer.findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
  auto fx_mix = m_editBuffer.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global });

  auto masterCP = master->getControlPositionValue();
  auto fxMixCP = fx_mix->getControlPositionValue();

  auto partVolumeCpVgI = VolumeConversionRules::getPartVolumeForConvertSingleToDual(masterCP, fxMixCP, VoiceGroup::I);
  auto partVolumeCpVgII = VolumeConversionRules::getPartVolumeForConvertSingleToDual(masterCP, fxMixCP, VoiceGroup::II);

  auto partVolI = m_editBuffer.findParameterByID({ C15::PID::Part_Volume, VoiceGroup::I });
  auto partVolII = m_editBuffer.findParameterByID({ C15::PID::Part_Volume, VoiceGroup::II });

  partVolI->setCPFromHwui(transaction, partVolumeCpVgI);
  partVolII->setCPFromHwui(transaction, partVolumeCpVgII);
}

void EditBufferConverter::initVoiceGroupMasterAndTune(UNDO::Transaction *transaction, VoiceGroup voiceGroup) const
{
  auto vgVolume = m_editBuffer.findParameterByID({ C15::PID::Part_Volume, voiceGroup });
  auto vgTune = m_editBuffer.findParameterByID({ C15::PID::Part_Tune, voiceGroup });
  vgVolume->loadDefault(transaction, Defaults::FactoryDefault);
  vgTune->loadDefault(transaction, Defaults::FactoryDefault);
}

void EditBufferConverter::undoableConvertSplitToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  combineSplitPartGlobalMaster(transaction, copyFrom);

  if(copyFrom != VoiceGroup::I)
  {
    ScopedMonophonicParameterLock effectsLock(transaction, m_editBuffer);
    ScopedLock lock2(transaction);
    lock2.addLock({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
    lock2.addLock({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
    lock2.addLock({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
    lock2.addLock({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });

    m_editBuffer.copyVoiceGroup(transaction, copyFrom, VoiceGroup::I);
  }

  appendConvertedSuffixToName(transaction, copyFrom);
}

void EditBufferConverter::appendConvertedSuffixToName(UNDO::Transaction *transaction, VoiceGroup &copyFrom)
{
  if(!StringTools::hasEnding(m_editBuffer.getVoiceGroupName(copyFrom), "conv."))
  {
    m_editBuffer.setName(transaction, m_editBuffer.getVoiceGroupName(copyFrom) + " conv.");
  }
}

void EditBufferConverter::undoableConvertLayerToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  combineLayerPartGlobalMaster(transaction, copyFrom);

  ScopedLock unisonAndMonolocks(transaction);
  unisonAndMonolocks.addGroupLock({ "Unison", VoiceGroup::I });
  unisonAndMonolocks.addGroupLock({ "Mono", VoiceGroup::I });

  ScopedMonophonicParameterLock fxLock(transaction, m_editBuffer);
  ScopedLock lock2(transaction);
  lock2.addLock({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  lock2.addLock({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
  lock2.addLock({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
  lock2.addLock({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });

  if(copyFrom != VoiceGroup::I)
    m_editBuffer.copyVoiceGroup(transaction, copyFrom, VoiceGroup::I);

  appendConvertedSuffixToName(transaction, copyFrom);
}

void EditBufferConverter::undoableConvertSingleToDualWithFXIOnly(UNDO::Transaction *transaction, SoundType type)
{
  SendEditBufferScopeGuard scopeGuard(transaction, true);
  m_editBuffer.undoableSetTypeFromConvert(transaction, type);
  setVoiceGroupNames(transaction, m_editBuffer.getName());

  m_editBuffer.copyVoiceGroup(transaction, VoiceGroup::I, VoiceGroup::II);

  copyPartTuneFromMasterTuneAndDefaultMasterGroup(transaction);
  initBothFadeFrom(transaction);
  initSplitPoint(transaction);
  initCrossFB(transaction);

  auto toFXI = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
  auto toFXII = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
  toFXI->loadDefault(transaction, Defaults::FactoryDefault);
  toFXII->loadDefault(transaction, Defaults::FactoryDefault);

  auto masterSerialFX = m_editBuffer.findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global });
  auto masterFXPan = m_editBuffer.findParameterByID({ C15::PID::Master_Pan, VoiceGroup::Global });

  masterSerialFX->setCPFromHwui(transaction, 0);
  masterFXPan->setCPFromHwui(transaction, 0);

  undoableUnmuteLayers(transaction);
  initRecallSetUUIDAndDoDeferredJobs(transaction);
}

void EditBufferConverter::initRecallSetUUIDAndDoDeferredJobs(UNDO::Transaction *transaction)
{
  m_editBuffer.initRecallValues(transaction);
  transaction->addUndoSwap(&m_editBuffer, m_editBuffer.m_lastLoadedPreset, Uuid::converted());
  m_editBuffer.m_deferredJobs.trigger();
}

void EditBufferConverter::copyPartVolumesToGlobalMasterAndFXMixForConvertDualToSingle(UNDO::Transaction *transaction,
                                                                                      VoiceGroup copyFrom)
{
  const auto masterVolume = m_editBuffer.findParameterByID({ C15::PID::Master_Volume, VoiceGroup::Global });
  const auto masterFX_MIX = m_editBuffer.findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global });
  const auto partVolI = m_editBuffer.findParameterByID({ C15::PID::Part_Volume, VoiceGroup::I });
  const auto partVolII = m_editBuffer.findParameterByID({ C15::PID::Part_Volume, VoiceGroup::II });

  const auto partVolumeSelf = m_editBuffer.findParameterByID({ C15::PID::Part_Volume, copyFrom });

  // handle edge cases without ratio
  if(partVolII->getControlPositionValue() == 0.0)
  {
    masterFX_MIX->setCPFromHwui(transaction, 0);
  }
  else if(partVolI->getControlPositionValue() == partVolII->getControlPositionValue())
  {
    masterFX_MIX->setCPFromHwui(transaction, 0.5);
  }
  else
  {
    const double ratio = VolumeConversionRules::parabolicGainCpToAmplitude(partVolI->getControlPositionValue())
        / VolumeConversionRules::parabolicGainCpToAmplitude(partVolII->getControlPositionValue());
    auto value = (std::sqrt((ratio * ratio) - ratio + 1.0) - ratio) / (1.0 - ratio);
    masterFX_MIX->setCPFromHwui(transaction, value);
  }
  const auto masterVolumeAmplitude
      = VolumeConversionRules::parabolicGainCpToAmplitude(masterVolume->getControlPositionValue());
  // determine master volume according to selected part
  if(copyFrom == VoiceGroup::I)
  {
    // handle edge case without amplitudes
    if(masterFX_MIX->getControlPositionValue() == 1.0)
    {
      masterVolume->setCPFromHwui(transaction, 0);
    }
    else
    {
      auto value
          = std::min(VolumeConversionRules::amplitudeToParabolicGainCp(
                         masterVolumeAmplitude
                         * VolumeConversionRules::parabolicGainCpToAmplitude(partVolumeSelf->getControlPositionValue())
                         / VolumeConversionRules::parabolicFadeCpToAmplitude(masterFX_MIX->getControlPositionValue())),
                     1.0);
      masterVolume->setCPFromHwui(transaction, value);
    }
  }
  else
  {
    // handle edge case without amplitudes
    if(masterFX_MIX->getControlPositionValue() == 0.0)
    {
      masterVolume->setCPFromHwui(transaction, 0);
    }
    else
    {
      auto value = std::min(
          VolumeConversionRules::amplitudeToParabolicGainCp(
              masterVolumeAmplitude
              * VolumeConversionRules::parabolicGainCpToAmplitude(partVolumeSelf->getControlPositionValue())
              / VolumeConversionRules::parabolicFadeCpToAmplitude(1.0 - masterFX_MIX->getControlPositionValue())),
          1.0);
      // handle general case by normalizing amplitudes
      masterVolume->setCPFromHwui(transaction, value);
    }
  }
}

void EditBufferConverter::copyToFXAndFxFrom(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  if(copyFrom == VoiceGroup::II)
  {
    auto to_fx_I = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I });
    auto to_fx_II = m_editBuffer.findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::II });
    to_fx_I->setCPFromHwui(transaction, 1 - to_fx_II->getControlPositionValue());

    auto from_fx_I = m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
    auto from_fx_II = m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });
    from_fx_I->setCPFromHwui(transaction, 1 - from_fx_II->getControlPositionValue());
  }
}

void EditBufferConverter::copyPartTuneFromMasterTuneAndDefaultMasterGroup(UNDO::Transaction *transaction)
{
  auto globalMasterGroup = m_editBuffer.getParameterGroupByID({ "Master", VoiceGroup::Global });
  auto partI = m_editBuffer.getParameterGroupByID({ "Part", VoiceGroup::I });
  auto partII = m_editBuffer.getParameterGroupByID({ "Part", VoiceGroup::II });

  //Copy Tune
  for(auto &ids : std::vector<std::pair<int, int>> { { C15::PID::Part_Tune, C15::PID::Master_Tune } })
  {
    auto pI = partI->findParameterByID({ ids.first, VoiceGroup::I });
    auto pII = partII->findParameterByID({ ids.first, VoiceGroup::II });
    auto pGlobal = globalMasterGroup->findParameterByID({ ids.second, VoiceGroup::Global });

    pI->copyFrom(transaction, pGlobal);
    pII->copyFrom(transaction, pGlobal);
  }

  {
    ScopedLock lock(transaction);
    lock.addLock({ C15::PID::Master_Pan, VoiceGroup::Global });
    lock.addLock({ C15::PID::Master_Serial_FX, VoiceGroup::Global });

    for(auto &parameter : globalMasterGroup->getParameters())
    {
      parameter->loadDefault(transaction, Defaults::FactoryDefault);
    }
  }
}

void EditBufferConverter::initSplitPoint(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    auto splitPoint = m_editBuffer.findParameterByID({ C15::PID::Split_Split_Point, vg });
    splitPoint->loadDefault(transaction, Defaults::FactoryDefault);
  }
}

void EditBufferConverter::initCrossFB(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    for(auto param : getCrossFBParameters(vg))
    {
      param->loadDefault(transaction, Defaults::FactoryDefault);
    }
  }
}

void EditBufferConverter::initCrossFBExceptFromFX(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    for(auto param : getCrossFBParameters(vg))
    {
      if(param->getID().getNumber() != C15::PID::FB_Mix_FX_Src)
      {
        param->loadDefault(transaction, Defaults::FactoryDefault);
      }
    }
  }
}

void EditBufferConverter::undoableUnisonMonoLoadDefaults(UNDO::Transaction *transaction, VoiceGroup vg)
{
  m_editBuffer.getParameterGroupByID({ "Mono", vg })->forEachParameter([&](auto p) -> void {
    p->loadDefault(transaction, Defaults::FactoryDefault);
  });
  m_editBuffer.getParameterGroupByID({ "Unison", vg })->forEachParameter([&](auto p) -> void {
    p->loadDefault(transaction, Defaults::FactoryDefault);
  });
}

void EditBufferConverter::setVoiceGroupNames(UNDO::Transaction *transaction, const Glib::ustring &newName)
{
  m_editBuffer.setVoiceGroupName(transaction, newName, VoiceGroup::I);
  m_editBuffer.setVoiceGroupName(transaction, newName, VoiceGroup::II);
}

void EditBufferConverter::initBothFadeFrom(UNDO::Transaction *transaction)
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
  {
    initFadeParameters(transaction, vg);
  }
}

void EditBufferConverter::applyConversionRuleForFBMixFXFromSingleToDual(UNDO::Transaction *transaction)
{
  auto parameterI = m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I });
  auto parameterII = m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::II });
  parameterII->setCPFromHwui(transaction, 1 - parameterI->getControlPositionValue());
}

void EditBufferConverter::calculateSplitPointFromFadeParams(UNDO::Transaction *transaction)
{
  auto idFromI = ParameterId { C15::PID::Part_Fade_From, VoiceGroup::I };
  auto idFromII = ParameterId { C15::PID::Part_Fade_From, VoiceGroup::II };
  const auto fadeFromICP = m_editBuffer.findParameterByID(idFromI)->getControlPositionValue();
  const auto fadeFromIICP = m_editBuffer.findParameterByID(idFromII)->getControlPositionValue();

  const auto meanFadeFrom = (fadeFromICP + fadeFromIICP) / 2.0;

  auto splitI = m_editBuffer.findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto splitII = m_editBuffer.findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II });

  splitI->setCPFromHwui(transaction, meanFadeFrom);
  splitII->stepCPFromHwui(transaction, 1, {});
}

void EditBufferConverter::defaultFadeParameters(UNDO::Transaction *transaction)
{
  for(auto &vg : { VoiceGroup::I, VoiceGroup::II })
  {
    m_editBuffer.findParameterByID({ C15::PID::Part_Fade_From, vg })
        ->loadDefault(transaction, Defaults::FactoryDefault);
    m_editBuffer.findParameterByID({ C15::PID::Part_Fade_Range, vg })
        ->loadDefault(transaction, Defaults::FactoryDefault);
  }
}

void EditBufferConverter::combineSplitPartGlobalMaster(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  auto masterGroup = m_editBuffer.getParameterGroupByID({ "Master", VoiceGroup::Global });

  auto originVolume = m_editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Part_Volume, copyFrom });
  auto originTune = m_editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Part_Tune, copyFrom });

  auto masterVolumeParameter
      = masterGroup->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Master_Volume, VoiceGroup::Global });
  auto masterTuneParameter
      = masterGroup->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Master_Tune, VoiceGroup::Global });

  undoableUnmuteLayers(transaction);

  auto newTune = originTune->getControlPositionValue() + masterTuneParameter->getControlPositionValue();

  masterVolumeParameter->setModulationAmount(transaction, originVolume->getModulationAmount());
  masterVolumeParameter->setModulationSource(transaction, originVolume->getModulationSource());

  masterTuneParameter->copyFrom(transaction, originTune);
  masterTuneParameter->setCPFromHwui(transaction, newTune);
}

void EditBufferConverter::combineLayerPartGlobalMaster(UNDO::Transaction *transaction, VoiceGroup copyFrom)
{
  auto masterGroup = m_editBuffer.getParameterGroupByID({ "Master", VoiceGroup::Global });

  auto originVolume = m_editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Part_Volume, copyFrom });
  auto originTune = m_editBuffer.findAndCastParameterByID<ModulateableParameter>({ C15::PID::Part_Tune, copyFrom });

  auto masterVolumeParameter
      = masterGroup->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Master_Volume, VoiceGroup::Global });
  auto masterTuneParameter
      = masterGroup->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Master_Tune, VoiceGroup::Global });

  undoableUnmuteLayers(transaction);

  auto newTune = originTune->getControlPositionValue() + masterTuneParameter->getControlPositionValue();

  masterVolumeParameter->setModulationSource(transaction, originVolume->getModulationSource());
  masterVolumeParameter->setModulationAmount(transaction, originVolume->getModulationAmount());

  masterTuneParameter->setCPFromHwui(transaction, newTune);
  masterTuneParameter->setModulationSource(transaction, originTune->getModulationSource());
  masterTuneParameter->setModulationAmount(transaction, originTune->getModulationAmount());
}
