#pragma once

#include <http/RPCActionManager.h>
#include <presets/EditBuffer.h>
#include <http/ContentSection.h>
#include <http/SectionAndActionManager.h>
#include "HWUI.h"
#include "use-cases/VoiceGroupAndLoadToPartManager.h"

class Application;

class HWUIActions : public SectionAndActionManager
{
 public:
  HWUIActions(UpdateDocumentContributor* parent, HWUI& hwui, EditBuffer& eb, VoiceGroupAndLoadToPartManager& vgManager);
};
