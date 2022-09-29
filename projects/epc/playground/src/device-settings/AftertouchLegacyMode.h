#include <device-settings/BooleanSetting.h>

class AftertouchLegacyMode : public BooleanSetting
{
 public:
  AftertouchLegacyMode(UpdateDocumentContributor& settings);
  void syncExternals(SendReason reason) const override;
};
