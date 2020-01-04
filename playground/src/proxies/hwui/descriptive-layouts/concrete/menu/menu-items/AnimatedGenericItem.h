#pragma once

#include "GenericItem.h"
#include <sigc++/connection.h>
#include <chrono>

class Animator
{
 public:
  using ProgressCB = std::function<void()>;
  using FinishedCB = std::function<void()>;

  Animator(std::chrono::milliseconds length, ProgressCB pcb, FinishedCB fcb);
  virtual ~Animator();
  float getAnimationPosition() const;

 private:
  bool doAnimation();
  bool finishAnimation();

  sigc::connection m_signal;
  ProgressCB m_animationCB;
  FinishedCB m_animationFinishedCB;

  std::chrono::steady_clock::time_point m_animationStartedAt = std::chrono::steady_clock::now();
  std::chrono::milliseconds m_animationLength { 500 };
};

class AnimatedGenericItem : public GenericItem
{
 public:
  template <class tCap, class tCB, class tCB2>
  AnimatedGenericItem(tCap caption, const Rect &r, tCB cb, tCB2 onAnimationFinishedCB = nullptr)
      : GenericItem(caption, r, cb)
  {
    m_animationFinishedCB = onAnimationFinishedCB;
  }


  void startAnimation();
  bool drawAnimationZug(FrameBuffer &buffer);
  bool redraw(FrameBuffer &fb) override;
  void doAction() override;

 private:
  std::unique_ptr<Animator> m_animator;
  std::function<void(void)> m_animationFinishedCB;
};
