#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/overlays/SplitPointOverlay.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/controls/SplitPointLabel.h>
#include "SplitPointItem.h"

ScrollMenuOverlay *SplitPointItem::createOverlay() {
  return new SplitPointOverlay(getStandardOverlayRect());
}

SplitPointItem::SplitPointItem() : EditorItem("Split Point") {
  auto rightHalf = getPosition();
  rightHalf.setWidth(rightHalf.getWidth() / 2);
  rightHalf.setLeft(rightHalf.getWidth());

  addControl(new SplitPointLabel(rightHalf));
}
