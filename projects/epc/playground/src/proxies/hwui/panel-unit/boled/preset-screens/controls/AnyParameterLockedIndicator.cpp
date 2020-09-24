#include "AnyParameterLockedIndicator.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/panel-unit/boled/BOLED.h"
#include <groups/ParameterGroup.h>

AnyParameterLockedIndicator::AnyParameterLockedIndicator(const Rect &pos)
    : LockedIndicator(Rect(pos.getX(), pos.getY(), pos.getWidth(), pos.getHeight()))
{
  Application::get().getPresetManager()->getEditBuffer()->onLocksChanged(
      sigc::mem_fun(this, &AnyParameterLockedIndicator::onParameterGroupChanged));
}

void AnyParameterLockedIndicator::onParameterGroupChanged()
{
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
  {
    for(auto g : Application::get().getPresetManager()->getEditBuffer()->getParameterGroups(vg))
    {
      if(g->isAnyParameterLocked())
      {
        setText("\ue20a");
        return;
      }
    }
  }
  setText("");
}
