#include <Application.h>
#include "HighlightChangedParametersSetting.h"

HighlightChangedParametersSetting::HighlightChangedParametersSetting(Settings& parent)
    : BooleanSetting(parent, true)
{
}

ForceHighlightChangedParametersSetting::ForceHighlightChangedParametersSetting(Settings& parent)
    : BooleanSetting(parent, false)
{
}

bool ForceHighlightChangedParametersSetting::persistent() const {
    return false;
}

void ForceHighlightChangedParametersSetting::enable() {
    Application::get()
            .getSettings()
            ->getSetting("ForceHighlightChangedParameters")
            ->setSetting(Initiator::EXPLICIT_HWUI, "on");
}

void ForceHighlightChangedParametersSetting::disable() {
    Application::get()
            .getSettings()
            ->getSetting("ForceHighlightChangedParameters")
            ->setSetting(Initiator::EXPLICIT_HWUI, "off");
}
