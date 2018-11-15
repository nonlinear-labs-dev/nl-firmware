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

LowerModulationBoundControl::~LowerModulationBoundControl()
{
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
      auto srcValue = mcParam->getControlPositionValue();
      double denominator = calcDominator(modifiers, modulatedParam);
      auto newLeft = (round(range.first * denominator) + inc) / denominator;
      newLeft = ScaleConverter::getControlPositionRangeUnipolar().clip(newLeft);

      auto newModAmount = range.second - newLeft;
      auto newValue = newLeft + newModAmount * srcValue;

      auto &undoScope = modulatedParam->getUndoScope();
      auto scope = undoScope.startContinuousTransaction(modulatedParam, "Set '%0'",
                                                        modulatedParam->getGroupAndParameterName());
      auto transaction = scope->getTransaction();

      modulatedParam->undoableSetModAmount(transaction, newModAmount);

      if(modulatedParam->isBiPolar())
        modulatedParam->setCPFromHwui(transaction, newValue * 2 - 1);
      else
        modulatedParam->setCPFromHwui(transaction, newValue);

      return true;
    }
  }
  return false;
}

double LowerModulationBoundControl::calcDominator(const ButtonModifiers &modifiers,
                                                  const ModulateableParameter *modulatedParam) const
{
  return modulatedParam->getModAmountDenominator(modifiers);
}
