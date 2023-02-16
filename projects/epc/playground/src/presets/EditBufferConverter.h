#pragma once

#include "libundo/undo/Transaction.h"
#include "nltools/Types.h"
#include "parameter_declarations.h"
#include "EditBufferModifierSharedBase.h"

class EditBuffer;
class Parameter;

class EditBufferConverter : public EditBufferModifierSharedBase
{
 public:
  using EditBufferModifierSharedBase::EditBufferModifierSharedBase;
  //convert with both fx
  void convertToDual(UNDO::Transaction *transaction, SoundType newType, VoiceGroup currentVG);
  void undoableConvertDualToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom);

  //convert with FX I
  void undoableConvertSingleToDualWithFXIOnly(UNDO::Transaction *transaction, SoundType type);

 private:
  // single to dual
  void undoableConvertSingleToLayer(UNDO::Transaction *transaction, VoiceGroup copyFrom);
  void undoableConvertSingleToSplit(UNDO::Transaction *transaction, VoiceGroup copyFrom);
  //dual to dual
  void undoableConvertLayerToSplit(UNDO::Transaction *transaction);
  void undoableConvertSplitToLayer(UNDO::Transaction *transaction, VoiceGroup copyFrom);
  //dual to single
  void undoableConvertSplitToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom);
  void undoableConvertLayerToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom);

  // implementation
  void undoableUnmuteLayers(UNDO::Transaction *transaction);
  void undoableInvertToFXII(UNDO::Transaction *transaction) const;
  void copyGlobalMasterAndFXMixToPartVolumesForConvertSingleToDual(UNDO::Transaction *transaction);
  void copyPartVolumesToGlobalMasterAndFXMixForConvertDualToSingle(UNDO::Transaction *transaction, VoiceGroup copyFrom);
  void copyToFXAndFxFrom(UNDO::Transaction *transaction, VoiceGroup copyFrom);
  void copyPartTuneFromMasterTuneAndDefaultMasterGroup(UNDO::Transaction *transaction);
  void initSplitPoint(UNDO::Transaction *transaction);
  void initCrossFB(UNDO::Transaction *transaction);
  void initCrossFBExceptFromFX(UNDO::Transaction *transaction);
  void undoableUnisonMonoLoadDefaults(UNDO::Transaction *transaction, VoiceGroup vg);
  void initVoiceGroupMasterAndTune(UNDO::Transaction *transaction, VoiceGroup voiceGroup) const;
  void setVoiceGroupNames(UNDO::Transaction *transaction, const Glib::ustring &newName);
  void initBothFadeFrom(UNDO::Transaction *transaction);
  void applyConversionRuleForFBMixFXFromSingleToDual(UNDO::Transaction *transaction);
  void calculateSplitPointFromFadeParams(UNDO::Transaction *transaction);
  void defaultFadeParameters(UNDO::Transaction *transaction);
  void combineSplitPartGlobalMaster(UNDO::Transaction *transaction, VoiceGroup copyFrom);
  void combineLayerPartGlobalMaster(UNDO::Transaction *transaction, VoiceGroup copyFrom);

  // helpers
  [[nodiscard]] std::vector<Parameter *> getParametersToLock(SoundType newType) const;
  [[nodiscard]] std::vector<C15::Descriptors::ParameterType> getMonophonicTypes() const;

  void appendConvertedSuffixToName(UNDO::Transaction *transaction, VoiceGroup &copyFrom);

  void initRecallSetUUIDAndDoDeferredJobs(UNDO::Transaction *transaction);
};
