#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/controls/OverlayParameterLabel.h>
#include "MonoParameterOverlay.h"
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <Application.h>

std::pair<Rect, Rect> splitHalf(const Rect &rect)
{
  auto left = rect;
  auto right = rect;
  left.setWidth(rect.getWidth() / 2);
  right.setWidth(rect.getWidth() / 2);
  right.setLeft(left.getWidth());
  return std::make_pair(left, right);
}

MonoParameterOverlay::MonoParameterOverlay(const Rect &r, const Parameter *parameter)
    : ArrowIncrementDecrementOverlay(r)
    , m_parameter{ parameter }
{
  auto pos = getPosition();
  pos.setLeft(10);
  pos.setWidth(pos.getWidth() - 20);
  addControl(new OverlayParameterLabel(pos, parameter));
}

void MonoParameterOverlay::onLeft(bool down)
{
  ArrowIncrementDecrementOverlay::onLeft(down);
  if(down)
    if(auto p = getParameter())
    {
      auto scope = Application::get().getPresetManager()->getUndoScope().startTransaction("Step Mono Param");
      auto transaction = scope->getTransaction();
      p->stepCPFromHwui(transaction, -1, {});
    }
}

void MonoParameterOverlay::onRight(bool down)
{
  ArrowIncrementDecrementOverlay::onRight(down);
  if(down)
    if(auto p = getParameter())
    {
      auto scope = Application::get().getPresetManager()->getUndoScope().startTransaction("Step Mono Param");
      auto transaction = scope->getTransaction();
      p->stepCPFromHwui(transaction, 1, {});
    }
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

Parameter *MonoParameterOverlay::getParameter() const
{
  return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(m_parameter->getID(),
                                                                                   m_parameter->getVoiceGroup());
}
