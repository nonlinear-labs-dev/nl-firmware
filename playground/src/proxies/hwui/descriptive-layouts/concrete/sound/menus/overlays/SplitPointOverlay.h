#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/ArrowIncrementDecrementOverlay.h>

class SplitPointLabel;

class SplitPointOverlay : public ArrowIncrementDecrementOverlay
{
 public:
  explicit SplitPointOverlay(const Rect& r);

  void onLeft(bool down) override;
  void onRight(bool down) override;
  void onCommit(bool down) override;

 private:
  SplitPointLabel* m_label;
};
