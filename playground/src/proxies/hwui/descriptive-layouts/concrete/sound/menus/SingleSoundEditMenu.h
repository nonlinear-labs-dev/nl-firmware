#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/ScrollMenu.h>
#include "SoundEditMenu.h"

class SingleSoundEditMenu : public SoundEditMenu {
 public:
  explicit SingleSoundEditMenu(const Rect& r);
  void init() override;
};
