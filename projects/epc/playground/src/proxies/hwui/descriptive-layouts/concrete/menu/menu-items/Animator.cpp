#include "Animator.h"
#include "Application.h"

Animator::Animator(std::chrono::milliseconds length, ProgressCB pcb, FinishedCB fcb)
    : m_animationCB(std::move(pcb))
    , m_animationFinishedCB(std::move(fcb))
    , m_animationLength(length)
{
  m_signal = Application::get().getMainContext()->signal_idle().connect(sigc::mem_fun(this, &Animator::doAnimation));
}

Animator::~Animator()
{
  m_signal.disconnect();
}

float Animator::getAnimationPosition() const  // 0 ... 1
{
  auto now = std::chrono::steady_clock::now();
  auto diff = now - m_animationStartedAt;
  return std::clamp(1.0f * diff / m_animationLength, 0.0f, 1.0f);
}

bool Animator::doAnimation()
{
  m_animationCB();

  auto diff = std::chrono::steady_clock::now() - m_animationStartedAt;

  if(diff > m_animationLength + std::chrono::milliseconds(100))
  {
    m_signal
        = Application::get().getMainContext()->signal_idle().connect(sigc::mem_fun(this, &Animator::finishAnimation));
    return false;
  }
  return true;
}

bool Animator::finishAnimation()
{
  m_animationFinishedCB();
  return false;
}