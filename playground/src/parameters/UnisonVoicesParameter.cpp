#include "UnisonVoicesParameter.h"
#include <parameters/scale-converters/LinearCountScaleConverter.h>
#include <parameters/scale-converters/dimension/VoicesDimension.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>

UnisonVoicesParameter::UnisonVoicesParameter(ParameterGroup* group, VoiceGroup vg)
    : UnmodulateableUnisonParameter(group, ParameterId { 249, vg },
                                    ScaleConverter::get<LinearCountScaleConverter<24, VoicesDimension>>(), 0, 23, 23)
{
}

bool UnisonVoicesParameter::shouldWriteDocProperties(UpdateDocumentContributor::tUpdateID knownRevision) const
{
  auto ret = Parameter::shouldWriteDocProperties(knownRevision) || didScalingChange();

  if(ret)
    m_scalingChanged = false;

  return ret;
}

void UnisonVoicesParameter::updateScaling(UNDO::Transaction* transaction, SoundType type)
{
  auto oldCP = getControlPositionValue();

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

    if(oldCP >= 0.5)
      setCpValue(transaction, Initiator::INDIRECT, 1.0, false);
    else
    {
      if(oldVoices == 1)
        setCpValue(transaction, Initiator::INDIRECT, oldCP * 2, false);
      else
      {
        setCPFromHwui(transaction, 0);
        stepCPFromHwui(transaction, oldVoices - 1, {});
      }
    }
  }

  m_scalingChanged = true;
  onChange();
}

bool UnisonVoicesParameter::didScalingChange() const
{
  return m_scalingChanged;
}
