#include <Application.h>
#include <device-settings/PitchbendOnPressedKey.h>
#include <proxies/lpc/LPCProxy.h>

PitchbendOnPressedKey::PitchbendOnPressedKey(UpdateDocumentContributor &settings)
    : super(settings, false)
{
}

PitchbendOnPressedKey::~PitchbendOnPressedKey()
{
}

void PitchbendOnPressedKey::sendToLPC(SendReason reason) const
{
#warning UNUSED Feature
}
