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

  if(fine)
    setText({ rangeDisplayValues.second, " F" });
  else
    setText({ rangeDisplayValues.second });
}
