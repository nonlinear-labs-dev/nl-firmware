#pragma once

#include "libundo/undo/Transaction.h"
#include "nltools/Types.h"
#include "ParameterId.h"
#include "EditBufferModifierSharedBase.h"

class EditBuffer;
class Preset;
class Parameter;

class EditBufferToPartLoader : public EditBufferModifierSharedBase
{
 public:
  using EditBufferModifierSharedBase::EditBufferModifierSharedBase;

  void undoableLoadPresetPartIntoPart(UNDO::Transaction* transaction, const Preset* preset, VoiceGroup from,
                                      VoiceGroup copyTo);

 private:
  void undoableLoadPresetPartIntoSplitSound(UNDO::Transaction* transaction, const Preset* preset, VoiceGroup from,
                                            VoiceGroup copyTo);
  void undoableLoadPresetPartIntoLayerSound(UNDO::Transaction* transaction, const Preset* preset, VoiceGroup copyFrom,
                                            VoiceGroup copyTo);
  void loadSinglePresetIntoSplitPart(UNDO::Transaction* transaction, const Preset* preset, VoiceGroup from,
                                     VoiceGroup loadInto);
  void loadSinglePresetIntoLayerPart(UNDO::Transaction* transaction, const Preset* preset, VoiceGroup from,
                                     VoiceGroup loadTo);

  std::vector<ParameterId> getFeedbackParametersLockedForLoadToPart(VoiceGroup loadTo);

  void copySinglePresetMasterToPartMaster(UNDO::Transaction* transaction, const Preset* preset, VoiceGroup targetGroup);
  void loadPartFromDualIntoSplit(UNDO::Transaction* transaction, VoiceGroup loadFrom, VoiceGroup loadTo,
                                 const Preset* preset);
  void copySumOfMasterGroupToVoiceGroupMasterGroup(UNDO::Transaction* transaction, const Preset* preset,
                                                   VoiceGroup copyFrom, VoiceGroup copyTo);
  void initFadeParameters(UNDO::Transaction* transaction, VoiceGroup group);
  void loadPartFromDualIntoLayer(UNDO::Transaction* transaction, VoiceGroup loadFrom, VoiceGroup loadTo,
                                 const Preset* preset);
  void copyPolyParametersFromI(UNDO::Transaction* transaction, const Preset* preset, VoiceGroup group);
  void initFadeFrom(UNDO::Transaction* transaction, VoiceGroup vg);
  void loadPartFromSingleIntoDual(UNDO::Transaction* transaction, VoiceGroup loadFrom, VoiceGroup loadTo,
                                  const Preset* preset);
  void loadPartFromDualIntoDual_OutputMixer(UNDO::Transaction* transaction, VoiceGroup loadFrom, VoiceGroup loadTo,
                                            const Preset* preset);
  double parabolicLevelAttenuation(const double controlPosition, const double fadePosition);
  void updateLoadFromPartOrigin(UNDO::Transaction* transaction, const Preset* preset, const VoiceGroup& from,
                                const VoiceGroup& loadTo);
  void copySpecialToFXParamForLoadSingleIntoDualPart(UNDO::Transaction* transaction, VoiceGroup from, VoiceGroup to,
                                                     const Preset* preset);
};
