#include "RandomizeItem.h"
#include <Application.h>
#include <device-settings/RandomizeAmount.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/SettingLabel.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/overlays/RandomizeOverlay.h>

RandomizeItem::RandomizeItem(const Rect& rect)
    : EditorItem("Randomize", rect)
{
  auto rightHalf = getPosition();
  rightHalf.setWidth(rightHalf.getWidth() / 2);
  rightHalf.setLeft(rightHalf.getWidth());

  addControl(new SettingLabel<RandomizeAmount>(rightHalf));
}

RandomizeItem::~RandomizeItem() = default;

MenuOverlay* RandomizeItem::createOverlay()
{
  return new RandomizeOverlay(getDefaultOverlayRect());
}
