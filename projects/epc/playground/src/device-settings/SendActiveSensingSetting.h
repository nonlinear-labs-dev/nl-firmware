#pragma once
#include <device-settings/BooleanSetting.h>

class SendActiveSensingSetting : public BooleanSetting
{
 public:
  explicit SendActiveSensingSetting(UpdateDocumentContributor& settings);
};
