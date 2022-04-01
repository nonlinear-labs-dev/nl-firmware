#include "PhysicalControlParameterUseCases.h"

PhysicalControlParameterUseCases::PhysicalControlParameterUseCases(PhysicalControlParameter *p)
    : ParameterUseCases(p)
    , m_physicalParam { p }
{
}

void PhysicalControlParameterUseCases::changeFromPlaycontroller(double value, HWChangeSource src)
{
  m_physicalParam->onChangeFromPlaycontroller(value, src);
}
