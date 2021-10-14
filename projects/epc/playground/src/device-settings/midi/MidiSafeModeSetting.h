#pragma once
#include <device-settings/BooleanSetting.h>

class MidiSafeModeSetting : public BooleanSetting
{
 public:
  MidiSafeModeSetting(UpdateDocumentContributor &settings);
};