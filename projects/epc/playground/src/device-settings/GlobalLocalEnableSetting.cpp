#include "GlobalLocalEnableSetting.h"
#include "nltools/GenericScopeGuard.h"

GlobalLocalEnableSetting::GlobalLocalEnableSetting(UpdateDocumentContributor& s)
    : BooleanSetting(s, true)
{
}
