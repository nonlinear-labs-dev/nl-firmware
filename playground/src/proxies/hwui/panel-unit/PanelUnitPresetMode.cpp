#include <Application.h>
#include <device-settings/DebugLevel.h>
#include <parameters/Parameter.h>
#include <parameters/MacroControlParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <proxies/hwui/panel-unit/PanelUnitPresetMode.h>
#include <proxies/hwui/TwoStateLED.h>
#include <memory>
#include "PanelUnitPresetMode.h"

PanelUnitPresetMode::PanelUnitPresetMode()
    : m_bruteForceLedThrottler(std::chrono::milliseconds(40))
{
  DebugLevel::gassy(__PRETTY_FUNCTION__);

  Application::get().getHWUI()->onModifiersChanged(
      sigc::hide(sigc::mem_fun(this, &PanelUnitPresetMode::bruteForceUpdateLeds)));

  Application::get().getPresetManager()->getEditBuffer()->onChange(
      mem_fun(this, &PanelUnitPresetMode::bruteForceUpdateLeds));
}

PanelUnitPresetMode::~PanelUnitPresetMode()
{
  DebugLevel::gassy(__PRETTY_FUNCTION__);
}

void PanelUnitPresetMode::bruteForceUpdateLeds()
{
  m_bruteForceLedThrottler.doTask([this]() {
    if(Application::get().getHWUI()->getPanelUnit().getUsageMode().get() != this)
      return;

    array<TwoStateLED::LedState, numLeds> states{ TwoStateLED::OFF };

    if(Application::get().getHWUI()->getButtonModifiers()[SHIFT] == true)
      getMappings().forEachButton(
          [&](int buttonId, const list<int> parameters) { letChangedButtonsBlink(buttonId, parameters, states); });
    else
      getMappings().forEachButton(
          [&](int buttonId, const list<int> parameters) { setStateForButton(buttonId, parameters, states); });

    applyStateToLeds(states);
  });
}

void PanelUnitPresetMode::letChangedButtonsBlink(int buttonId, const list<int> parameters,
                                                 array<TwoStateLED::LedState, numLeds>& states)
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  auto currentParams = getMappings().findParameters(buttonId);
  auto ebParameters = editBuffer->getParametersSortedById();

  bool anyChanged = false;
  for(auto paramID : currentParams)
  {
    anyChanged |= ebParameters[paramID]->isChangedFromLoaded();
  }
  states[buttonId] = anyChanged ? TwoStateLED::BLINK : TwoStateLED::OFF;
}

void PanelUnitPresetMode::setStateForButton(int buttonId, const list<int> parameters,
                                            array<TwoStateLED::LedState, numLeds>& states)
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();

  for(auto i : parameters)
  {
    auto signalFlowIndicator = ParameterDB::get().getSignalPathIndication(i);
    auto parameter = editBuffer->findParameterByID(static_cast<size_t>(i));

    if(auto mc = dynamic_cast<MacroControlParameter*>(parameter))
    {
      if(!mc->getTargets().empty())
      {
        states[buttonId] = TwoStateLED::ON;
        break;
      }
    }
    else if(isSpecialCase(parameter->getID()))
    {
      auto sig = getSpecialCaseValue(parameter->getID());
      if(sig != parameter->getControlPositionValue())
      {
        states[buttonId] = TwoStateLED::ON;
        break;
      }
    }
    else if(signalFlowIndicator != invalidSignalFlowIndicator)
    {
      if(parameter->getControlPositionValue() != signalFlowIndicator)
      {
        states[buttonId] = TwoStateLED::ON;
        break;
      }
    }
  }
}

void PanelUnitPresetMode::applyStateToLeds(array<TwoStateLED::LedState, numLeds>& states)
{
  auto& panelUnit = Application::get().getHWUI()->getPanelUnit();
  for(int i = 0; i < numLeds; i++)
  {
    panelUnit.getLED(i)->setState(states[i]);
  }
}

const tControlPositionValue PanelUnitPresetMode::getSpecialCaseValue(const gint32 id) const
{
  switch(id)
  {
    case 307:
      return 0;
    default:
      return ParameterDB::getInvalidSignalPathIndication();
  }
}

const bool PanelUnitPresetMode::isSpecialCase(const gint32 id) const {
  auto special = std::vector<size_t>{307};
  return std::find(special.begin(), special.end(), id) != special.end();
}
