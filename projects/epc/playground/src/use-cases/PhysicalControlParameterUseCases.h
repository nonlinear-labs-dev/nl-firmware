#pragma once
#include <parameters/PhysicalControlParameter.h>
#include <use-cases/ParameterUseCases.h>

class PhysicalControlParameterUseCases : public ParameterUseCases
{
 public:
  explicit PhysicalControlParameterUseCases(PhysicalControlParameter* p);
  void changeFromPlaycontroller(double value, HWChangeSource src);

 private:
  PhysicalControlParameter* m_physicalParam;
};
