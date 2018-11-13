#include <Application.h>
#include <glibmm/ustring.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/LowerModulationBoundLabel.h>
#include <utility>
#include <tools/StringTools.h>

class MacroControlParameter;

LowerModulationBoundLabel::LowerModulationBoundLabel(const Rect &r)
    : super(r)
{
}

LowerModulationBoundLabel::~LowerModulationBoundLabel()
{
}

void LowerModulationBoundLabel::updateText(MacroControlParameter *mcParam, ModulateableParameter *modulatedParam)
{
  auto rangeDisplayValues = modulatedParam->getModRangeAsDisplayValues();
  const auto fine = isHighlight() && Application::get().getHWUI()->isModifierSet(ButtonModifier::FINE);
  auto displayValue = rangeDisplayValues.first + (fine ? " F" : "");

  auto textClipped = false;

  if(getFont()->getStringWidth(displayValue) >= getWidth())
  {
    displayValue = StringTools::removeSpaces(displayValue);
    textClipped = true;
  }

  if(fine)
    setText({ displayValue, textClipped ? 1 : 2 });
  else
    setText(displayValue);
}
