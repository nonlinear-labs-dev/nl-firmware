#include "PhysicalControlParameterUseCases.h"
#include <libundo/undo/Scope.h>

PhysicalControlParameterUseCases::PhysicalControlParameterUseCases(PhysicalControlParameter *p)
    : ParameterUseCases(p)
    , m_physicalParam { p }
{
}

void PhysicalControlParameterUseCases::changeFromPlaycontroller(double value, HWChangeSource src)
{
  if(m_physicalParam->isLocalEnabled() || src == HWChangeSource::MIDI)
  {
    m_physicalParam->onChangeFromPlaycontroller(value, src);
  }
  else if(auto send = m_physicalParam->getSendParameter())
  {
    auto scope = UNDO::Scope::startTrashTransaction();
    auto trans = scope->getTransaction();
    send->setCPFromHwui(trans, value);
  }
}
