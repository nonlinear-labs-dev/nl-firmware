#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/EditorItem.h>

class SplitPointItem : public EditorItem {
public:
  SplitPointItem(const Rect &rect);
  MenuOverlay *createOverlay() override;
};
