#include "PhysicalControlParameterUseCases.h"
#include "parameters/PedalParameter.h"
#include <libundo/undo/Scope.h>

PhysicalControlParameterUseCases::PhysicalControlParameterUseCases(PhysicalControlParameter *p)
    : ParameterUseCases(p)
    , m_physicalParam { p }
{
}

void PhysicalControlParameterUseCases::changeFromAudioEngine(double value, HWChangeSource src)
{
  if(m_physicalParam->isLocalEnabled() || src == HWChangeSource::MIDI)
  {
    m_physicalParam->onChangeFromExternalSource(value, src);
  }
  else if(auto send = m_physicalParam->getSendParameter())
  {
    auto scope = UNDO::Scope::startTrashTransaction();
    auto trans = scope->getTransaction();
    send->setCPFromHwui(trans, value);
  }
}

void PhysicalControlParameterUseCases::setIndirect(tControlPositionValue position)
{
  auto scope = UNDO::Scope::startTrashTransaction();
  m_physicalParam->setIndirect(scope->getTransaction(), position);
}

bool PhysicalControlParameterUseCases::applyPolledHWPosition(float value)
{
    if(value != std::numeric_limits<float>::max())
    {
      auto dValue = static_cast<double>(value);

      if(m_physicalParam->getValue().differs(dValue))
      {
        if(auto pedal = dynamic_cast<PedalParameter*>(m_physicalParam))
        {
          if(pedal->getReturnMode() == ReturnMode::None)
          {
            setIndirect(dValue);
            return true;
          }
        }
        changeFromAudioEngine(dValue, HWChangeSource::TCD);
        return true;
      }
    }
    return false;
}
