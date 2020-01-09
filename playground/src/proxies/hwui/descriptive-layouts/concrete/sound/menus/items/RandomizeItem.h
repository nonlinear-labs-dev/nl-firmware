#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/EditorItem.h>

class RandomizeItem : public EditorItem
{
 public:
  RandomizeItem(const Rect& rect);
  ~RandomizeItem() override;
  MenuOverlay* createOverlay() override;
};
