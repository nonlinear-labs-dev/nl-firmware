#include "AnimatedGenericItem.h"
#include <glibmm/main.h>
#include <Application.h>
#include <nltools/logging/Log.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/Oleds.h>

#include <utility>

Animator::Animator(std::chrono::milliseconds length, ProgressCB pcb, FinishedCB fcb, CatchAllCB cacb)
    : m_animationCB(std::move(pcb))
    , m_animationFinishedCB(std::move(fcb))
    , m_animationCatchAllCB(std::move(cacb))
    , m_animationLength(length)
{
  m_signal = Application::get().getMainContext()->signal_idle().connect(sigc::mem_fun(this, &Animator::doAnimation));
}

Animator::~Animator()
{
  m_animationCatchAllCB();
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

void AnimatedGenericItem::startAnimation()
{
  if(!m_animator)
    m_animator = std::make_unique<Animator>(
        std::chrono::milliseconds(500), [this] { this->setDirty(); },
        [this] {
          this->setDirty();

          m_animator.reset();

          if(m_finish.cb.has_value())
            m_finish.cb.value()();
        }, [this](){
          if(m_finally.cb.has_value())
            m_finally.cb.value()();
        });
}

bool AnimatedGenericItem::drawAnimationZug(FrameBuffer &buffer)
{
  if(!m_animator)
    return false;

  auto pos = getPosition();
  auto p = m_animator->getAnimationPosition();
  float newWidth = p * static_cast<float>(pos.getWidth());
  pos.setWidth(static_cast<int>(newWidth));
  buffer.setColor(FrameBufferColors::C255);
  buffer.fillRect(pos);
  return true;
}

bool AnimatedGenericItem::redraw(FrameBuffer &fb)
{
  auto ret = ControlWithChildren::redraw(fb);
  ret |= drawEnterIndication(fb);
  ret |= drawAnimationZug(fb);
  ret |= drawHighlightBorder(fb);
  return ret;
}

void AnimatedGenericItem::doAction()
{
  startAnimation();
  doStartCB();
}

void AnimatedGenericItem::doStartCB()
{
  if(m_start.cb.has_value())
  {
    m_start.cb.value()();
  }
}