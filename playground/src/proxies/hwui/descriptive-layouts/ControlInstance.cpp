#include <utility>

#include <utility>

#include <utility>

#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MacroControlEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationCarousel.h>
#include <proxies/hwui/panel-unit/boled/sound-screens/controls/ConvertSoundMenu.h>
#include <proxies/hwui/panel-unit/boled/sound-screens/controls/SingleSoundEditMenuLegacy.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterCarousel.h>

#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetList.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Preset/LoadVoiceGroupPresetList.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/SplitSoundEditMenu.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/SingleSoundEditMenu.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Sound/Menus/LayerSoundEditMenu.h>
#include "ControlInstance.h"
#include "GenericControl.h"

#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterEditButtonMenu.h"
#include "ControlRegistry.h"

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

  Control* ControlInstance::instantiate() const
  {
    if(ControlRegistry::get().exists(controlClass))
    {
      auto control = ControlRegistry::get().instantiateConcrete(controlClass, position);
      return control ? control : new GenericControl(*this);
    }
    else
    {
      return new GenericControl(*this);
    }
  }
}
