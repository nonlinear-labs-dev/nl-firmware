#include "AnimatedGenericItem.h"
#include <glibmm/main.h>
#include <Application.h>
#include <nltools/logging/Log.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/Oleds.h>

#include <utility>

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