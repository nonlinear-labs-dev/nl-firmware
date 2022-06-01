#include "AftertouchLegacyMode.h"
#include "Application.h"
#include "proxies/playcontroller/PlaycontrollerProxy.h"

AftertouchLegacyMode::AftertouchLegacyMode(UpdateDocumentContributor& settings)
    : BooleanSetting(settings, false)
{
}

void AftertouchLegacyMode::syncExternals(SendReason reason) const
{
  Application::get().getPlaycontrollerProxy()->sendSetting(PLAYCONTROLLER_SETTING_ID_ENABLE_LEGACY_AT, gint16(get()));
}
