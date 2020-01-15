#include <Application.h>
#include "HighlightChangedParametersSetting.h"

HighlightChangedParametersSetting::HighlightChangedParametersSetting(UpdateDocumentContributor& parent)
    : BooleanSetting(parent, true)
{
}

ForceHighlightChangedParametersSetting::ForceHighlightChangedParametersSetting(UpdateDocumentContributor& parent)
    : BooleanSetting(parent, false)
{
}

bool ForceHighlightChangedParametersSetting::persistent() const
{
  return false;
}
