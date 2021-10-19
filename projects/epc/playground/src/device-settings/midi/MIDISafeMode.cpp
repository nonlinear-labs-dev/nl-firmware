#include "MIDISafeMode.h"

MIDISafeMode::MIDISafeMode(UpdateDocumentContributor &settings)
    : BooleanSetting(settings, true)
{
}

