#include <presets/EditBuffer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include "ConvertToSingleItem.h"

ConvertToSingleItem::ConvertToSingleItem()
    : AnimatedGenericItem(
          "Convert to Single", [] { Application::get().getPresetManager()->getEditBuffer()->setType(Type::Single); },
          [] {
            Application::get().getHWUI()->setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
          })
{
}
