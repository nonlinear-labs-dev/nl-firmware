#pragma once
#include <parameters/PhysicalControlParameter.h>
#include <use-cases/ParameterUseCases.h>

class PhysicalControlParameterUseCases : public ParameterUseCases
{
 public:
  explicit PhysicalControlParameterUseCases(PhysicalControlParameter* p);
  void changeFromAudioEngine(double value, HWChangeSource src);

  void setIndirect(tControlPositionValue position);
  bool applyPolledHWPosition(float value);
  void stepBehaviour(int inc);

 private:
  PhysicalControlParameter* m_physicalParam;
};
