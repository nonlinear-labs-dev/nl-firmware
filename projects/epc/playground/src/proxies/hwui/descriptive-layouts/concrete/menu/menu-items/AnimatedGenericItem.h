#pragma once

#include "GenericItem.h"
#include <sigc++/connection.h>
#include <chrono>
#include <proxies/hwui/panel-unit/boled/setup/OneShotEntryTypes.h>
#include "Animator.h"

class AnimatedGenericItem : public GenericItem
{
 public:
  template <class tCap>
  AnimatedGenericItem(tCap caption, const Rect &r, OneShotTypes::StartCB start, OneShotTypes::FinishCB finish)
      : GenericItem(caption, r, [] {})
  {
    m_start = std::move(start);
    m_finish = std::move(finish);
  }

  void startAnimation();
  bool drawAnimationZug(FrameBuffer &buffer);
  bool redraw(FrameBuffer &fb) override;
  void doAction() override;

 private:
  void doStartCB();
  std::unique_ptr<Animator> m_animator;
  OneShotTypes::StartCB m_start;
  OneShotTypes::FinishCB m_finish;
};
