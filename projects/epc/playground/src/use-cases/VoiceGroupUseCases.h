#pragma once
#include "nltools/Types.h"

class EditBuffer;
class VoiceGroupAndLoadToPartManager;

class VoiceGroupUseCases
{
 public:
  VoiceGroupUseCases(VoiceGroupAndLoadToPartManager* manager, EditBuffer* eb);
  void setVoiceGroup(VoiceGroup vg);
  void setVoiceGroupSilent(VoiceGroup vg);
  void toggleVoiceGroupSelection();
  void toggleVoiceGroupSelectionSilent();
 private:
  VoiceGroupAndLoadToPartManager* m_manager;
  EditBuffer* m_editBuffer;
};
