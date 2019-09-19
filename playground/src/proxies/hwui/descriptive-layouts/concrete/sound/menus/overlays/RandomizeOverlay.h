#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/ArrowIncrementDecrementOverlay.h>
#include <device-settings/RandomizeAmount.h>
#include "proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/ChangeSettingWithCommitOverlay.h"

class RandomizeOverlay : public ChangeSettingWithCommitOverlay<RandomizeAmount>
{
 public:
  explicit RandomizeOverlay(const Rect& r);
};