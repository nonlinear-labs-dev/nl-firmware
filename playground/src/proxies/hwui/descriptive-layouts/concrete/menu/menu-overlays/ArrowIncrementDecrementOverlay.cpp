#include <tools/Uuid.h>
#include "ArrowIncrementDecrementOverlay.h"

ArrowIncrementDecrementOverlay::ArrowIncrementDecrementOverlay(const Rect& r)
    : LeftRightUpDownCommitOverlay(r)
{

  auto fullWidth = r.getWidth();
  auto labelWidth = fullWidth - 20;

  m_leftArrow = addControl(new ArrowLeft({ 0, -2, 10, 13 }));
  m_rightArrow = addControl(new ArrowRight({ labelWidth + 10, -2, 10, 13 }));
}

void ArrowIncrementDecrementOverlay::onLeft(bool down) {
  m_leftArrow->setHighlight(down);
}

void ArrowIncrementDecrementOverlay::onRight(bool down) {
  m_rightArrow->setHighlight(down);
}

