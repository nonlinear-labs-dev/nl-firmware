#include <tools/Uuid.h>
#include "ArrowIncrementDecrementOverlay.h"
#include <Application.h>
#include <proxies/hwui/FrameBuffer.h>

ArrowIncrementDecrementOverlay::ArrowIncrementDecrementOverlay(const Rect& r)
    : LeftRightUpDownCommitOverlay(r)
{
  auto fullWidth = r.getWidth();
  auto labelWidth = fullWidth - 30;

  m_leftArrow = addControl(new ArrowLeft({ 0, -2, 10, 13 }));
  m_rightArrow = addControl(new ArrowRight({ labelWidth + 10, -2, 10, 13 }));
  m_enterSymbol = addControl(new Label({ "\u23CE", 0 }, { labelWidth + 21, -3, 10, 13 }));
}

void ArrowIncrementDecrementOverlay::onLeft(bool down)
{
  m_leftArrow->setHighlight(down);

  if(down)
    installResetHighlight(m_leftArrow);
}

bool ArrowIncrementDecrementOverlay::drawAnimationZug(FrameBuffer& fb)
{
  if(!m_animator)
    return false;

  auto pos = getPosition();
  auto p = m_animator->getAnimationPosition();
  const auto fullWidth = Rect { 0, 0, 256, 12 };
  float newWidth = p * static_cast<float>(fullWidth.getWidth());
  pos.setWidth(static_cast<int>(newWidth));
  pos.setLeft(0);
  fb.setColor(FrameBufferColors::C255);
  fb.fillRect(pos);
  return true;
}

bool ArrowIncrementDecrementOverlay::redraw(FrameBuffer& fb)
{
  MenuOverlay::redraw(fb);
  return drawAnimationZug(fb);
}

void ArrowIncrementDecrementOverlay::onRight(bool down)
{
  m_rightArrow->setHighlight(down);

  if(down)
    installResetHighlight(m_rightArrow);
}

void ArrowIncrementDecrementOverlay::onCommit(bool down)
{
  if(!m_animator && down)
  {
    m_animator = std::make_unique<Animator>(
        std::chrono::milliseconds(500), [this] { this->setDirty(); },
        [this] {
          this->setDirty();
          m_animator.reset();
        });

    m_enterSymbol->setHighlight(true);
    installResetHighlight(m_enterSymbol);
  }
}

void ArrowIncrementDecrementOverlay::installResetHighlight(Control* controlToReset)
{
  using std::chrono::milliseconds;

  m_resetHighlight = std::make_unique<Expiration>(
      Application::get().getMainContext(), [=]() { controlToReset->setHighlight(false); }, milliseconds { 150 });
}
