#pragma once
#include "parameters/unison-parameters/UnmodulateableUnisonParameter.h"

class UnisonVoicesParameter : public UnmodulateableUnisonParameter
{
 public:
  explicit UnisonVoicesParameter(ParameterGroup *group, VoiceGroup vg);
  void updateScaling(SoundType type);

  void copyFrom(UNDO::Transaction *transaction, const PresetParameter *other) override;
  void copyTo(UNDO::Transaction *transaction, PresetParameter *other) const override;

 protected:
  bool shouldWriteDocProperties(tUpdateID knownRevision) const override;

  mutable bool m_scalingChanged = false;
};
