#include <nltools/Assert.h>
#include "ParameterUseCases.h"
#include "presets/EditBuffer.h"

ParameterUseCases::ParameterUseCases(Parameter* parameter)
    : m_parameter { parameter }
{
  nltools_assertAlways(m_parameter != nullptr);
}

void ParameterUseCases::recallParameterFromPreset()
{
  if(m_parameter->isChangedFromLoaded())
  {
    m_parameter->undoableRecallFromPreset();
  }
}
