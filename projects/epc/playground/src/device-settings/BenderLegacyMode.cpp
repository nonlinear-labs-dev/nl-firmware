#include "BenderLegacyMode.h"
#include "Application.h"
#include <proxies/playcontroller/PlaycontrollerProxy.h>

BenderLegacyMode::BenderLegacyMode(UpdateDocumentContributor& settings)
    : BooleanSetting(settings, false)
{
}

void BenderLegacyMode::syncExternals(SendReason reason) const
{
  Application::get().getPlaycontrollerProxy()->sendSetting(PLAYCONTROLLER_SETTING_ID_ENABLE_LEGACY_BNDR, gint16(get()));
}
