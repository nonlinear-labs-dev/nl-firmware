#include "SensitiveBenderSettling.h"
#include "Application.h"
#include <proxies/playcontroller/PlaycontrollerProxy.h>

SensitiveBenderSettling::SensitiveBenderSettling(UpdateDocumentContributor& settings)
    : BooleanSetting(settings, false)
{
}

void SensitiveBenderSettling::syncExternals(SendReason reason) const
{
  guint16 state = get();
  Application::get().getPlaycontrollerProxy()->sendSetting(PLAYCONTROLLER_SETTING_ID_BNDR_SETTLING_SENSITIVITY, state);
}
