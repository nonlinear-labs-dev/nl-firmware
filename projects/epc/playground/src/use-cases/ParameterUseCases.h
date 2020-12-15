#pragma once
#include <ParameterId.h>

class EditBuffer;
class Parameter;

class ParameterUseCases
{
 public:
  ParameterUseCases(const ParameterId& id, EditBuffer* eb);
  void recallParameterFromPreset();

 private:
  Parameter* m_parameter = nullptr;
};