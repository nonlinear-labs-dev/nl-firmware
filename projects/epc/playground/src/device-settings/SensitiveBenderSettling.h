#include <device-settings/BooleanSetting.h>

class SensitiveBenderSettling : public BooleanSetting
{
 public:
  SensitiveBenderSettling(UpdateDocumentContributor& settings);
  void syncExternals(SendReason reason) const override;
};
