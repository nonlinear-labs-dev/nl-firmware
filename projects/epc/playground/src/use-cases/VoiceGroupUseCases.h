#pragma once
#include "nltools/Types.h"

class EditBuffer;
class VoiceGroupAndLoadToPartManager;

class VoiceGroupUseCases
{
 public:
  VoiceGroupUseCases(VoiceGroupAndLoadToPartManager* manager, EditBuffer* eb);
  void setVoiceGroup(VoiceGroup vg);
  void toggleVoiceGroupSelection();
 private:
  VoiceGroupAndLoadToPartManager* m_manager;
  EditBuffer* m_editBuffer;
};
