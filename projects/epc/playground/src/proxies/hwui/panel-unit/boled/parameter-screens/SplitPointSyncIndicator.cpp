#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/SplitPointSyncParameters.h>
#include <proxies/hwui/Oleds.h>
#include "SplitPointSyncIndicator.h"

SplitPointSyncIndicator::SplitPointSyncIndicator(const Rect& r)
    : LabelRegular8("\uE20C", r)
{
  Application::get().getSettings()->getSetting<SplitPointSyncParameters>()->onChange(
      sigc::mem_fun(this, &SplitPointSyncIndicator::onChange));
}

void SplitPointSyncIndicator::onChange(const Setting* s)
{
  if(auto sync = dynamic_cast<const SplitPointSyncParameters*>(s))
    setVisible(sync->get());
}
