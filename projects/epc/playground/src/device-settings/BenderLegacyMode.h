#include <device-settings/BooleanSetting.h>

class BenderLegacyMode : public BooleanSetting
{
 public:
  BenderLegacyMode(UpdateDocumentContributor& settings);
  void syncExternals(SendReason reason) const override;
};
