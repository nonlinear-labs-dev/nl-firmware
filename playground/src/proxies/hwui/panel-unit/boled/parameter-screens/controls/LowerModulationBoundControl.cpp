#include <Application.h>
#include <groups/MacroControlsGroup.h>
#include <libundo/undo/Scope.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/LowerModulationBoundControl.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/LowerModulationBoundLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/LowerModulationBoundSlider.h>
#include <functional/LambdaFactory.h>

LowerModulationBoundControl::LowerModulationBoundControl(const Rect &r)
    : super(r)
{
  auto height = r.getHeight() / 2;

  addControl(new LowerModulationBoundLabel(Rect(0, 0, r.getWidth(), height)));
  m_dummyButton = addControl(new Button("", Rect(0, r.getHeight() - 1 - 11, 58, 11)));
  addControl(new LowerModulationBoundSlider(Rect(0, height, r.getWidth(), height)));

  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      mem_fun(this, &LowerModulationBoundControl::onSelectionChanged));
}

void LowerModulationBoundControl::onSelectionChanged(Parameter *, Parameter *newParam)
{
  m_paramChanged.disconnect();

  if(newParam)
    m_paramChanged = newParam->onParameterChanged(mem_fun(this, &LowerModulationBoundControl::onParameterChanged));
}

void LowerModulationBoundControl::onParameterChanged(const Parameter *p)
{
  if(auto a = dynamic_cast<const ModulateableParameter *>(p))
    m_dummyButton->setVisible(a->getModulationSource() == ModulateableParameter::NONE);
  else
    m_dummyButton->setVisible(true);

  setDirty();
}

bool LowerModulationBoundControl::onRotary(int inc, ButtonModifiers modifiers)
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();

  if(auto modulatedParam = dynamic_cast<ModulateableParameter *>(editBuffer->getSelected()))
  {
    auto mc = modulatedParam->getModulationSource();
    auto mcID = MacroControlsGroup::modSrcToParamID(mc);

    if(auto mcParam = dynamic_cast<MacroControlParameter *>(editBuffer->findParameterByID(mcID)))
    {
      auto range = modulatedParam->getModulationRange(true);

      if(modulatedParam->isBiPolar())
      {
        range.first = 2 * range.first - 1;
        range.second = 2 * range.second - 1;
      }

      auto srcValue = mcParam->getControlPositionValue();
      double denominator = modifiers[FINE] ? modulatedParam->getValue().getFineDenominator()
                                           : modulatedParam->getValue().getCoarseDenominator();

      auto newLeft = (round(range.first * denominator) + inc) / denominator;
      newLeft = modulatedParam->getValue().getScaleConverter()->getControlPositionRange().clip(newLeft);
      auto newModAmount = range.second - newLeft;

      auto newValue = newLeft + newModAmount * srcValue;

      if(modulatedParam->isBiPolar())
        newModAmount /= 2;

      auto &undoScope = modulatedParam->getUndoScope();
      auto scope = undoScope.startContinuousTransaction(modulatedParam, "Set '%0'",
                                                        modulatedParam->getGroupAndParameterName());
      auto transaction = scope->getTransaction();

      modulatedParam->undoableSetModAmount(transaction, newModAmount);
      modulatedParam->setCPFromHwui(transaction, newValue);
      return true;
    }
  }
  return false;
}

bool LowerModulationBoundControl::onButton(int i, bool down, ButtonModifiers modifiers)
{
  switch(i)
  {
    case BUTTON_DEFAULT:
      if(down && LambdaFactory::setCurrentModulatedParamAmountToZero()())
        return true;
      break;
  }
  return false;
}
