#include "DefaultPresetSelectionEvents.h"

DescriptiveLayouts::DefaultPresetSelectionEvents::DefaultPresetSelectionEvents()
    : PresetSelectionEvents([](auto preset) { return true; })
{
}
