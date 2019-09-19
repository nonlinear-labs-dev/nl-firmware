#include "HWUIEvents.h"
#include "proxies/hwui/HWUIEnums.h"

void DescriptiveLayouts::HWUIEvents::isFineEventSource::onButtonModifiersChanged(const ButtonModifiers &mod)
{
  setValue(mod[ButtonModifier::FINE]);
}
