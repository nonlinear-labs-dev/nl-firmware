#include "UnisonVoicesParameter.h"
#include "parameter_declarations.h"
#include <parameters/scale-converters/LinearCountScaleConverter.h>
#include <parameters/scale-converters/dimension/VoicesDimension.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <presets/PresetParameter.h>
#include <algorithm>

UnisonVoicesParameter::UnisonVoicesParameter(ParameterGroup *group, VoiceGroup vg)
    : UnmodulateableUnisonParameter(group, ParameterId { C15::PID::Unison_Voices, vg },
                                    ScaleConverter::get<LinearCountScaleConverter<24, VoicesDimension>>(), 0, 23, 23)
{
}

bool UnisonVoicesParameter::shouldWriteDocProperties(UpdateDocumentContributor::tUpdateID knownRevision) const
{
  return std::exchange(m_scalingChanged, false) || Parameter::shouldWriteDocProperties(knownRevision);
}

void UnisonVoicesParameter::updateScaling(SoundType type)
{
  auto oldVoices = getDisplayValue();

  if(type == SoundType::Single)
  {
    getValue().setScaleConverter(ScaleConverter::get<LinearCountScaleConverter<24, VoicesDimension>>());
    getValue().setCoarseDenominator(23);
    getValue().setFineDenominator(23);
  }
  else
  {
    getValue().setScaleConverter(ScaleConverter::get<LinearCountScaleConverter<12, VoicesDimension>>());
    getValue().setCoarseDenominator(11);
    getValue().setFineDenominator(11);
  }

  getValue().setRawValue(Initiator::INDIRECT, 0);
  getValue().setRawValue(Initiator::INDIRECT, getNextStepValue(oldVoices, {}));
  m_scalingChanged = true;
  onChange();
}

void UnisonVoicesParameter::copyFrom(UNDO::Transaction *transaction, const PresetParameter *other)
{
  if(!isLocked())
  {
    if(static_cast<EditBuffer *>(getParentGroup()->getParent())->isDual())
    {
      auto numVoicesIn = other->getValue() * 23.0;
      auto res = std::clamp(numVoicesIn / 11.0, 0.0, 1.0);
      loadFromPreset(transaction, res);
    }
    else
    {
      UnmodulateableUnisonParameter::copyFrom(transaction, other);
    }
  }
}

void UnisonVoicesParameter::copyTo(UNDO::Transaction *transaction, PresetParameter *other) const
{
  if(static_cast<EditBuffer *>(getParentGroup()->getParent())->isDual())
  {
    auto numVoicesIn = getControlPositionValue() * 11.0;
    auto res = numVoicesIn / 23.0;
    other->setValue(transaction, res);
  }
  else
  {
    UnmodulateableUnisonParameter::copyTo(transaction, other);
  }
}
