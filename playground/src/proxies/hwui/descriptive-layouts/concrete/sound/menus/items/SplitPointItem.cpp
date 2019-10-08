#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/overlays/SplitPointOverlay.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/controls/SplitPointLabel.h>
#include "SplitPointItem.h"

MenuOverlay *SplitPointItem::createOverlay() {
  return new SplitPointOverlay(getDefaultOverlayRect());
}

SplitPointItem::SplitPointItem(const Rect &rect) : EditorItem("Split Point", rect) {
  auto rightHalf = getPosition();
  rightHalf.setWidth(rightHalf.getWidth() / 2);
  rightHalf.setLeft(rightHalf.getWidth());

  addControl(new SplitPointLabel(rightHalf));
}
