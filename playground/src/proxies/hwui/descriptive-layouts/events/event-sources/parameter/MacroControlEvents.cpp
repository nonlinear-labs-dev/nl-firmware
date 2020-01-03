#include "MacroControlEvents.h"
#include "proxies/hwui/descriptive-layouts/events/event-sources/base/EventSource.h"
#include <parameters/ModulateableParameter.h>
#include <parameters/MacroControlParameter.h>

void DescriptiveLayouts::CurrentMacroControlAssignment::onSelectedParameterChanged(const Parameter *p)
{
  if(auto modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    auto v = modP->getModulationSource() != MacroControls::NONE;
    setValue(v);
  }
}

void DescriptiveLayouts::CurrentMacroControlSymbol::onSelectedParameterChanged(const Parameter *p)
{
  if(const auto *modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    if(auto mc = modP->getMacroControl())
    {
      setValue(DisplayString(mc->getShortName(), 0));
      return;
    }
  }
  setValue(DisplayString("[-]", 0));
}

void DescriptiveLayouts::CurrentMacroControlAmount::onSelectedParameterChanged(const Parameter *p)
{
  if(const auto *modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    setValue(DisplayString(modP->stringizeModulationAmount(), 0));
  }
}

void DescriptiveLayouts::CurrentMacroControlPositionText::onSelectedParameterChanged(const Parameter *p)
{
  if(const auto *modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    if(auto mc = modP->getMacroControl())
    {
      setValue(DisplayString(mc->getDisplayString(), 0));
    }
  }
}

void DescriptiveLayouts::CurrentModParamModRangeEventSource::onSelectedParameterChanged(const Parameter *p)
{
  if(auto modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    auto lower = std::min(modP->getModulationRange(true).first, modP->getModulationRange(true).second);
    lower = std::max(0., lower);
    auto upper = std::max(modP->getModulationRange(true).first, modP->getModulationRange(true).second);
    upper = std::min(1., upper);
    setValue(std::make_pair(lower, upper));
  }
}

void DescriptiveLayouts::CurrentMacroControlPosition::onSelectedParameterChanged(const Parameter *p)
{
  if(auto modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    if(auto mc = modP->getMacroControl())
    {
      setValue(mc->getControlPositionValue());
      return;
    }
  }
}

void DescriptiveLayouts::MCPositionChanged::onSelectedParameterChanged(const Parameter *p)
{
  if(auto modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    setValue(modP->isMacroControlAssignedAndChanged());
  }
}

void DescriptiveLayouts::MCSelectionChanged::onSelectedParameterChanged(const Parameter *p)
{
  if(auto modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    setValue(modP->isModSourceChanged());
  }
}

void DescriptiveLayouts::MCAmountChanged::onSelectedParameterChanged(const Parameter *p)
{
  if(auto modP = dynamic_cast<const ModulateableParameter *>(p))
  {
    setValue(modP->isModAmountChanged());
  }
}
