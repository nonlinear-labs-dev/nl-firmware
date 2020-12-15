#pragma once
#include <nltools/Types.h>
#include "ParameterUseCases.h"
#include <memory>

class EditBuffer;
class Preset;

class EditBufferUseCases
{
 public:
  explicit EditBufferUseCases(EditBuffer* eb);

  void resetCustomScale();

  void selectParameter(const ParameterId& id);
  void selectParameter(const Parameter* param);

  void undoableLoad(const Preset* preset);
  void undoableLoadToPart(const Preset* preset, VoiceGroup from, VoiceGroup to);

  std::unique_ptr<ParameterUseCases> getUseCase(ParameterId id);

 private:
  EditBuffer* m_editBuffer;
};