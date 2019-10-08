#include <presets/EditBuffer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include "ConvertToSingleItem.h"

ConvertToSingleItem::ConvertToSingleItem(const Rect &rect)
    : AnimatedGenericItem(
          "Convert to Single", rect,
          [] {
            auto scope
                = Application::get().getPresetManager()->getUndoScope().startTransaction("Convert Sound to Single");
            auto transaction = scope->getTransaction();
            Application::get().getPresetManager()->getEditBuffer()->undoableConvertToSingle(transaction);
          },
          [] {
            Application::get().getHWUI()->setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
          })
{
}
