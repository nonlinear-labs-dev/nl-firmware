#include <groups/EnvelopeBGroup.h>
#include <parameters/ModulateableParameter.h>
#include "parameter_declarations.h"

EnvelopeBGroup::EnvelopeBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env B", vg }, "Envelope B", "Envelope B", "Envelope B")
{
}
