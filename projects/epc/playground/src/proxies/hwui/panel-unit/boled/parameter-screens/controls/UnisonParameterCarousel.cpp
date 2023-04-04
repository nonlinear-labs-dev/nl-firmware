#include "UnisonParameterCarousel.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>

std::vector<ParameterId> UnisonParameterCarousel::getParameterIdsForMode(SoundType type)
{
  auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();

  switch(type)
  {
    case SoundType::Split:
      return { { C15::PID::Unison_Voices, currentVG },
               { C15::PID::Unison_Detune, currentVG },
               { C15::PID::Unison_Phase, currentVG },
               { C15::PID::Unison_Pan, currentVG } };
    case SoundType::Layer:
    case SoundType::Single:
      return { { C15::PID::Unison_Voices, VoiceGroup::I },
               { C15::PID::Unison_Detune, VoiceGroup::I },
               { C15::PID::Unison_Phase, VoiceGroup::I },
               { C15::PID::Unison_Pan, VoiceGroup::I } };
  }

  return {};
}
