#include "AnyParameterLockedIndicator.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/panel-unit/boled/BOLED.h"

AnyParameterLockedIndicator::AnyParameterLockedIndicator(const Rect &pos) : LockedIndicator(Rect(pos.getX(), pos.getY(), pos.getWidth(), pos.getHeight()))
{
}

void AnyParameterLockedIndicator::onParameterSelected(Parameter * newOne)
{
    if(newOne) {
        m_groupConnection.disconnect();
        m_groupConnection = newOne->getParentGroup()->onGroupChanged(sigc::mem_fun(this, &AnyParameterLockedIndicator::onParameterGroupChanged));
    }
}

void AnyParameterLockedIndicator::onParameterGroupChanged() {
    auto anyParamLocked = false;
    for(auto g: Application::get().getPresetManager()->getEditBuffer()->getParameterGroups())
        if(g->isAnyParameterLocked()) {
            anyParamLocked = true;
            break;
        }
    setText(anyParamLocked ? "\ue20a" : "");
}

