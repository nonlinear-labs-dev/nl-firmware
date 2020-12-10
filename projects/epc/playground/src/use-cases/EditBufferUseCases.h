#pragma once
#include <nltools/Types.h>

class EditBuffer;
class Preset;

class EditBufferUseCases
{
 public:
  explicit EditBufferUseCases(EditBuffer* eb);

  void resetCustomScale();

  void undoableLoad(const Preset* preset);
  void undoableLoadToPart(const Preset* preset, VoiceGroup from, VoiceGroup to);

 private:
  EditBuffer* m_editBuffer;
};