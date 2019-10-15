#include "IndependentPresetSelectionEvents.h"
#include <presets/Preset.h>

DescriptiveLayouts::IndependentPresetSelectionEvents::IndependentPresetSelectionEvents()
    : PresetSelectionEvents([](auto preset) { return preset->getType() == SoundType::Single; })
{
}
