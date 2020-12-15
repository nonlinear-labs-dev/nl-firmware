#pragma once
#include <ParameterId.h>

class EditBuffer;
class Parameter;

class ParameterUseCases
{
 public:
  ParameterUseCases(Parameter* parameter);
  void recallParameterFromPreset();

 private:
  Parameter* m_parameter = nullptr;
};