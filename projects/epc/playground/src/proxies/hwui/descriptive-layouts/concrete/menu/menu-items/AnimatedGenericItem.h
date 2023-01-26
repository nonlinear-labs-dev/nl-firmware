#pragma once

#include "GenericItem.h"
#include <sigc++/connection.h>
#include <chrono>
#include <proxies/hwui/panel-unit/boled/setup/OneShotEntryTypes.h>

class Animator
{
 public:
  using ProgressCB = std::function<void()>;
  using FinishedCB = std::function<void()>;
  using CatchAllCB = std::function<void()>;

  Animator(std::chrono::milliseconds length, ProgressCB pcb, FinishedCB fcb, CatchAllCB cacb);
  virtual ~Animator();
  [[nodiscard]] float getAnimationPosition() const;

 private:
  bool doAnimation();
  bool finishAnimation();

  sigc::connection m_signal;
  ProgressCB m_animationCB;
  FinishedCB m_animationFinishedCB;
  CatchAllCB m_animationCatchAllCB;

  std::chrono::steady_clock::time_point m_animationStartedAt = std::chrono::steady_clock::now();
  std::chrono::milliseconds m_animationLength { 500 };
};

class AnimatedGenericItem : public GenericItem
{
 public:
  template <class tCap>
  AnimatedGenericItem(tCap caption, const Rect &r, OneShotTypes::StartCB start, OneShotTypes::FinishCB finish, OneShotTypes::CatchAllCB finally = OneShotTypes::CatchAllCB())
      : GenericItem(caption, r, [] {})
  {
    m_start = std::move(start);
    m_finish = std::move(finish);
    m_finally = std::move(finally);
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
  OneShotTypes::CatchAllCB m_finally;
};
