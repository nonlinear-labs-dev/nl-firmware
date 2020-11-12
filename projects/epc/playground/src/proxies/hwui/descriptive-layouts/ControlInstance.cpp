#include <utility>

#include <utility>

#include <utility>

#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MacroControlEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationCarousel.h>
#include <proxies/hwui/panel-unit/boled/sound-screens/controls/ConvertSoundMenu.h>
#include <proxies/hwui/panel-unit/boled/sound-screens/controls/SingleSoundEditMenuLegacy.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterCarousel.h>

#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetList.h>
#include <proxies/hwui/descriptive-layouts/concrete/preset/LoadVoiceGroupPresetList.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/DualSoundEditMenu.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/SingleSoundEditMenu.h>
#include "ControlInstance.h"
#include "GenericControl.h"

#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterEditButtonMenu.h"
#include "ControlRegistry.h"
#include "PropertyOwner.h"

namespace DescriptiveLayouts
{
  ControlInstance::ControlInstance(ControlInstances controlInstance, ControlClasses control, const Point& position,
                                   EventConnections eventConnections, StaticInitList staticInit)
      : controlInstance(std::move(controlInstance))
      , controlClass(std::move(control))
      , position(position)
      , eventConnections(std::move(eventConnections))
      , staticInitList(std::move(staticInit))
  {
  }

  Control* ControlInstance::instantiate(EventProvider* eventProvider) const
  {
    if(ControlRegistry::get().exists(controlClass))
      if(auto control = ControlRegistry::get().instantiateConcrete(controlClass, position))
        return control;

    return new GenericControl(*this, eventProvider);
  }
}
