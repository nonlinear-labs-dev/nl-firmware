#pragma once
#include <device-settings/BooleanSetting.h>

class MIDISafeMode : public BooleanSetting
{
 public:
  MIDISafeMode(UpdateDocumentContributor &settings);
};