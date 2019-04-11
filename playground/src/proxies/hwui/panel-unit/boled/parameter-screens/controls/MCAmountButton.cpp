#include "MCAmountButton.h"
#include "parameters/ModulateableParameter.h"
#include <proxies/hwui/buttons.h>

MCAmountButton::MCAmountButton(int id)
    : super("MC Amt", id)
{
}

MCAmountButton::~MCAmountButton() = default;

void MCAmountButton::update(const Parameter *parameter)
{

  if(m_lastCastedUpdateParam == nullptr || m_lastCastedUpdateParam != parameter)
  {
      m_lastCastedUpdateParam = dynamic_cast<const ModulateableParameter*>(parameter);
  }

  if(m_lastCastedUpdateParam)
  {
    if(m_lastCastedUpdateParam->getModulationSource() == ModulationSource::NONE)
      setText("");
    else
      setText({ "MC Amt"s + (m_lastCastedUpdateParam->isModAmountChanged() ? "*" : "") });
  }
  else
  {
    setText("");
  }
}
