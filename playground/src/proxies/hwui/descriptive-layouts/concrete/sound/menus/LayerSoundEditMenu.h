#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/ScrollMenu.h>

class LayerSoundEditMenu : public ScrollMenu
{
 public:
  explicit LayerSoundEditMenu(const Rect& r);
  void init() override;
};
