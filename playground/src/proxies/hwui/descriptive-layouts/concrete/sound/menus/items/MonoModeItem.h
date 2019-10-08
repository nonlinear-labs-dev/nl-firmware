#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/EditorItem.h>

class MonoModeItem : public EditorItem
{
public:
  MonoModeItem(const Rect &rect);
  MenuOverlay *createOverlay() override;
};