#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/MenuOverlay.h>
#include <proxies/hwui/controls/ArrowRight.h>
#include <proxies/hwui/controls/ArrowLeft.h>
#include <proxies/hwui/controls/LabelRegular8.h>
#include "LeftRightUpDownCommitOverlay.h"
#include "proxies/hwui/descriptive-layouts/concrete/menu/menu-items/Animator.h"
#include <nltools/threading/Expiration.h>

class ArrowIncrementDecrementOverlay : public LeftRightUpDownCommitOverlay
{
 public:
  explicit ArrowIncrementDecrementOverlay(const Rect& r);

  void onLeft(bool down) override;
  void onRight(bool down) override;
  void onCommit(bool down) override;

  void onUp(bool down) override {};
  void onDown(bool down) override {};

  bool redraw(FrameBuffer& fb) override;
  bool drawAnimationZug(FrameBuffer& fb);

 private:
  std::unique_ptr<Animator> m_animator = nullptr;
  ArrowRight* m_rightArrow;
  ArrowLeft* m_leftArrow;
  Label* m_enterSymbol;

  void installResetHighlight(Control* controlToReset);

  std::unique_ptr<Expiration> m_resetHighlight;
};
