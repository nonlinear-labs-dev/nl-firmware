#include "UnisonParameterCarousel.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

std::vector<ParameterId> UnisonParameterCarousel::getParameterIdsForMode(SoundType type)
{
  auto currentVG = Application::get().getPresetManager()->getEditBuffer()->getCurrentHWUIVoiceGroup();

  switch(type)
  {
    case SoundType::Split:
      return { { 249, currentVG }, { 250, currentVG }, { 252, currentVG }, { 253, currentVG } };
    case SoundType::Layer:
    case SoundType::Single:
      return { { 249, VoiceGroup::I }, { 250, VoiceGroup::I }, { 252, VoiceGroup::I }, { 253, VoiceGroup::I } };
  }

  return {};
}
