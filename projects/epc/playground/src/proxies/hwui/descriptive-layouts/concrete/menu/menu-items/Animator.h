#pragma once
#include <functional>
#include <chrono>
#include <sigc++/sigc++.h>

class Animator
{
 public:
  using ProgressCB = std::function<void()>;
  using FinishedCB = std::function<void()>;

  Animator(std::chrono::milliseconds length, ProgressCB pcb, FinishedCB fcb);
  virtual ~Animator();
  [[nodiscard]] float getAnimationPosition() const;

 private:
  bool doAnimation();
  bool finishAnimation();

  sigc::connection m_signal;
  ProgressCB m_animationCB;
  FinishedCB m_animationFinishedCB;

  std::chrono::steady_clock::time_point m_animationStartedAt = std::chrono::steady_clock::now();
  std::chrono::milliseconds m_animationLength { 500 };
};