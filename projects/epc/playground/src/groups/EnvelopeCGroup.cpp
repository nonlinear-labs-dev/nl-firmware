#include <groups/EnvelopeCGroup.h>
#include <parameters/ModulateableParameter.h>
#include <parameter_declarations.h>

EnvelopeCGroup::EnvelopeCGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Env C", vg }, "Envelope C", "Envelope C", "Envelope C")
{
}