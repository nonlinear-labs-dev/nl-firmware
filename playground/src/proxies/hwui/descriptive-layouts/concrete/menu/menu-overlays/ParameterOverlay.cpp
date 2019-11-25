#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterValueLabel.h>
#include <Application.h>
#include "presets/EditBuffer.h"
#include "presets/PresetManager.h"
#include "ParameterOverlay.h"

ParameterOverlay::ParameterOverlay(const Rect& rect, Parameter * const parameter)
    : ArrowIncrementDecrementOverlay(rect)
    , m_parameter{ parameter }
{
  auto labelWidth = rect.getWidth() - 20;
  m_label = addControl(new ParameterValueLabel(parameter, { 11, -1, labelWidth, 13 }));
}

void ParameterOverlay::onLeft(bool down)
{
  ArrowIncrementDecrementOverlay::onLeft(down);
  auto param = getNonConstParameter(m_parameter);
  auto scope = Application::get().getPresetManager()->getUndoScope().startTransaction("TODO");
  auto transaction = scope->getTransaction();
  param->stepCPFromHwui(transaction, -1, {});
}

void ParameterOverlay::onRight(bool down)
{
  ArrowIncrementDecrementOverlay::onRight(down);
  auto param = getNonConstParameter(m_parameter);
  auto scope = Application::get().getPresetManager()->getUndoScope().startTransaction("TODO");
  auto transaction = scope->getTransaction();
  param->stepCPFromHwui(transaction, 1, {});
}

#warning "Adlerauge"
Parameter* ParameterOverlay::getNonConstParameter(const Parameter* param) const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  return eb->findParameterByID(param->getID());
}
