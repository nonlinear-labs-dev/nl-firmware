#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/overlays/MonoModeOverlay.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/MonoModeScrollMenuOverlay.h>
#include "MonoModeItem.h"

MonoModeItem::MonoModeItem(const Rect &rect)
    : EditorItem("Mono Mode", rect)
{
}

MenuOverlay *MonoModeItem::createOverlay()
{
  auto og = getPosition();
  auto third = og.getWidth() / 3;
  auto twoThirds
      = Rect(third + 1, 0, third * 2 + 1, 52);
  return new MonoModeScrollMenuOverlay(twoThirds);
}
