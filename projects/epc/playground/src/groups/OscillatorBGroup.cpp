#include "OscillatorBGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"

OscillatorBGroup::OscillatorBGroup(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Osc B", vg }, "Oscillator B", "Oscillator B", "Oscillator B")
{
}