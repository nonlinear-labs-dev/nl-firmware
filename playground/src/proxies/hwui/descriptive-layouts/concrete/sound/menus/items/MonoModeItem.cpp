#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/overlays/MonoModeOverlay.h>
#include "MonoModeItem.h"

MonoModeItem::MonoModeItem(const Rect &rect)
    : EditorItem("Mono Mode", rect)
{
}

MenuOverlay *MonoModeItem::createOverlay()
{
  return new MonoModeOverlay(getFullRightSideOverlayRect());
}
