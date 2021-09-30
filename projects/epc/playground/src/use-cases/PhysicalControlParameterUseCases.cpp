#include "PhysicalControlParameterUseCases.h"

PhysicalControlParameterUseCases::PhysicalControlParameterUseCases(PhysicalControlParameter *p)
    : ParameterUseCases(p)
    , m_physicalParam { p }
{
}

void PhysicalControlParameterUseCases::changeFromPlaycontroller(double value, bool shouldModulate)
{
  m_physicalParam->onChangeFromPlaycontroller(value, shouldModulate);
}
