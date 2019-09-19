#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/EditorItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/overlays/RandomizeOverlay.h>

class RandomizeItem : public EditorItem
{
 public:
  RandomizeItem();
  ~RandomizeItem() override;
  ScrollMenuOverlay* createOverlay() override;
};
