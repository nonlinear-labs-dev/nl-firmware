#include <presets/PresetParameter.h>
#include <parameters/Parameter.h>
#include <presets/Preset.h>
#include "parameters/ModulateableParameter.h"
#include "parameters/scale-converters/ParabolicGainDbScaleConverter.h"
#include "parameters/ScopedLock.h"
#include "SendEditBufferScopeGuard.h"
#include "EditBufferToPartLoader.h"
#include "EditBufferModifierSharedBase.h"
#include <presets/EditBuffer.h>

EditBufferModifierSharedBase::EditBufferModifierSharedBase(EditBuffer &eb)
    : m_editBuffer { eb }
{
}

VoiceGroup EditBufferModifierSharedBase::invert(VoiceGroup vg)
{
  return vg == VoiceGroup::I ? VoiceGroup::II : VoiceGroup::I;
}

std::vector<Parameter *> EditBufferModifierSharedBase::getCrossFBParameters(const VoiceGroup &to) const
{
  return std::vector<Parameter *>({ m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc, to }),
                                    m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Osc_Src, to }),
                                    m_editBuffer.findParameterByID({ C15::PID::FB_Mix_Comb_Src, to }),
                                    m_editBuffer.findParameterByID({ C15::PID::FB_Mix_SVF_Src, to }),
                                    m_editBuffer.findParameterByID({ C15::PID::FB_Mix_FX_Src, to }) });
}

void EditBufferModifierSharedBase::initFadeParameters(UNDO::Transaction *transaction, VoiceGroup group)
{
  auto f1 = m_editBuffer.findParameterByID({ C15::PID::Part_Fade_From, group });
  f1->loadDefault(transaction, Defaults::FactoryDefault);

  auto f2 = m_editBuffer.findParameterByID({ C15::PID::Part_Fade_Range, group });
  f2->loadDefault(transaction, Defaults::FactoryDefault);
}