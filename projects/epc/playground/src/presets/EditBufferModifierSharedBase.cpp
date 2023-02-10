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
