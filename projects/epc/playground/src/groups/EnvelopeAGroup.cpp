#include <groups/EnvelopeAGroup.h>
#include <parameters/ModulateableParameter.h>
#include "parameter_declarations.h"

EnvelopeAGroup::EnvelopeAGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env A", vg }, "Envelope A", "Envelope A", "Envelope A")
{
}