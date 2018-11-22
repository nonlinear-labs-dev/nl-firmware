#include "BenderRampBypass.h"
#include "Application.h"
#include "proxies/lpc/LPCProxy.h"
#include "Settings.h"

BenderRampBypass::BenderRampBypass(Settings& settings)
    : BooleanSetting(settings, false)
{
}

BenderRampBypass::~BenderRampBypass()
{
}

void BenderRampBypass::sendToLPC() const
{
  Application::get().getLPCProxy()->sendSetting(BENDER_RAMP_BYPASS, (uint16_t)(get()));
}

bool BenderRampBypass::persistent() const
{
  return false;
}
