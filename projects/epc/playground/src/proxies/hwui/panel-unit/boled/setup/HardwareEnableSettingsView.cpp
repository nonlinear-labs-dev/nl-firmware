#include "HardwareEnableSettingsView.h"
#include "HardwareEnableSettingsHelper.h"

HardwareEnableSettingsView::HardwareEnableSettingsView(tID id)
    : SetupLabel(Rect(0, 0, 0, 0))
{
  if(auto s = getSetting())
  {
    std::stringstream ss;
    for(auto aspect : getAspectsForIndex(id))
    {
      auto state = s->getState(id, aspect);
      ss << (state ? "X " : "O ");
    }
    setText({ ss.str() });
  }
}
