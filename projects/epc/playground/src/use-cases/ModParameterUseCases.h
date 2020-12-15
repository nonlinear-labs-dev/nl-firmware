#pragma once
#include <ParameterId.h>

class EditBuffer;
class ModulateableParameter;

class ModParameterUseCases
{
 public:
  ModParameterUseCases(const ParameterId& id, EditBuffer* eb);

  void undoableRecallMCPos();
  void recallMCAmount();

 private:
  ModulateableParameter* m_modParam = nullptr;
};
