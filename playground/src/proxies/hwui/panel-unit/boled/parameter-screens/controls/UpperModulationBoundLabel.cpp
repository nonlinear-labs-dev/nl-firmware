#include <Application.h>
#include <glibmm/ustring.h>
#include <parameters/ModulateableParameter.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UpperModulationBoundLabel.h>
#include <utility>
#include <tools/StringTools.h>

class MacroControlParameter;

UpperModulationBoundLabel::UpperModulationBoundLabel(const Rect &r)
    : super(r)
{
}

UpperModulationBoundLabel::~UpperModulationBoundLabel()
{
}

void UpperModulationBoundLabel::updateText(MacroControlParameter *mcParam, ModulateableParameter *modulatedParam)
{
  auto rangeDisplayValues = modulatedParam->getModRangeAsDisplayValues();
  const auto fine = isHighlight() && Application::get().getHWUI()->isModifierSet(ButtonModifier::FINE);
  auto displayValue = rangeDisplayValues.second + (fine ? " F" : "");

  auto textClipped = false;

  if(getFont()->getStringWidth(displayValue) >= 55)
  {
    displayValue = StringTools::removeSpaces(displayValue);
    textClipped = true;
  }

  if(fine)
    setText({ displayValue, textClipped ? 1 : 2 });
  else
    setText(displayValue);
}
