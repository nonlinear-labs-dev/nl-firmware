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
  using tTransaction = UNDO::Transaction;
  using EditBufferModifierSharedBase::EditBufferModifierSharedBase;

  void undoableLoadPresetPartIntoPart(tTransaction* transaction, const Preset* preset, VoiceGroup from,
                                      VoiceGroup copyTo);

 private:
  void loadDualPartIntoSplitSound(tTransaction* transaction, const Preset* preset, VoiceGroup from, VoiceGroup copyTo);
  void loadDualPartIntoLayerSound(tTransaction* transaction, const Preset* preset, VoiceGroup from, VoiceGroup copyTo);
  void loadSingleIntoSplitPart(tTransaction* transaction, const Preset* preset, VoiceGroup from, VoiceGroup loadInto);
  void loadSingleIntoLayerPart(tTransaction* transaction, const Preset* preset, VoiceGroup from, VoiceGroup loadTo);

  void loadPartFromDualIntoLayer(tTransaction* transaction, VoiceGroup from, VoiceGroup to, const Preset* preset);
  void loadPartFromDualIntoSplit(tTransaction* transaction, VoiceGroup from, VoiceGroup loadTo, const Preset* preset);
  void loadPartFromSingleIntoDual(tTransaction* transaction, VoiceGroup from, VoiceGroup to, const Preset* preset);

  std::vector<ParameterId> getFeedbackParametersLockedForLoadToPart(VoiceGroup loadTo);

  void copySinglePresetMasterToPartMaster(tTransaction* transaction, const Preset* preset, VoiceGroup targetGroup);
  void copySumOfPartMasterToPartMaster(tTransaction* t, const Preset* preset, VoiceGroup from, VoiceGroup to);

  void copyPolyParametersFromI(tTransaction* transaction, const Preset* preset, VoiceGroup group);
  void initFadeFrom(tTransaction* transaction, VoiceGroup vg);
  void applyLoadToPartOutMixerRule(tTransaction* transaction, VoiceGroup from, VoiceGroup to, const Preset* preset);
  void updateLoadFromPartOrigin(tTransaction* transaction, const Preset* preset, VoiceGroup from, VoiceGroup loadTo);
  void setInvertedOutMixFromII(tTransaction* transaction, VoiceGroup from, VoiceGroup to, const Preset* preset);

  double parabolicLevelAttenuation(double controlPosition, double fadePosition);
};
