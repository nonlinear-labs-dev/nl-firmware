#pragma once
#include <ParameterId.h>
#include <playground.h>
#include <parameters/Parameter.h>

class EditBuffer;
class Parameter;

class ParameterUseCases
{
 public:
  explicit ParameterUseCases(Parameter* parameter);

  void recallParameterFromPreset();
  void undoRecallParameterFromPreset(tControlPositionValue cp);

  void toggleLoadDefault();

  void incDec(int incs, bool fine, bool shift);
  void setDefault();

  void setControlPosition(tControlPositionValue cp);
  void loadDefault(Defaults defaults);

  void stepControlPosition(int steps, bool fine, bool shift);

 private:
  Parameter* m_parameter = nullptr;
};