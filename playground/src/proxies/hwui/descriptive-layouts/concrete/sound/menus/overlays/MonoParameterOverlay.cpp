#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/controls/OverlayParameterLabel.h>
#include "MonoParameterOverlay.h"

MonoParameterOverlay::MonoParameterOverlay(const Rect &r, const Parameter *parameter)
    : ArrowIncrementDecrementOverlay(r)
    , m_parameter{ parameter }
{
  addControl(new OverlayParameterLabel(getPosition(), parameter));
}

void MonoParameterOverlay::onLeft(bool down)
{
  ArrowIncrementDecrementOverlay::onLeft(down);
}

void MonoParameterOverlay::onRight(bool down)
{
  ArrowIncrementDecrementOverlay::onRight(down);
}

void MonoParameterOverlay::onUp(bool down)
{
  ArrowIncrementDecrementOverlay::onUp(down);
}

void MonoParameterOverlay::onDown(bool down)
{
  ArrowIncrementDecrementOverlay::onDown(down);
}

void MonoParameterOverlay::onCommit(bool down)
{
  ArrowIncrementDecrementOverlay::onCommit(down);
}
