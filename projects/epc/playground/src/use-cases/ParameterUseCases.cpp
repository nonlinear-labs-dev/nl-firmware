#include <nltools/Assert.h>
#include "ParameterUseCases.h"
#include "presets/EditBuffer.h"

ParameterUseCases::ParameterUseCases(const ParameterId& id, EditBuffer* eb)
    : m_parameter { eb->findParameterByID(id) }
{
  nltools_detailedAssertAlways(m_parameter != nullptr, "Could not create ParameterUseCases for id:" + id.toString());
}

void ParameterUseCases::recallParameterFromPreset()
{
  if(m_parameter->isChangedFromLoaded())
  {
    m_parameter->undoableRecallFromPreset();
  }
}
