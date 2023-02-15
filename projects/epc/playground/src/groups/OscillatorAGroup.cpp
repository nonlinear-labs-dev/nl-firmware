#include "OscillatorAGroup.h"
#include "parameters/ModulateableParameter.h"
#include "parameter_declarations.h"
#include "parameter_group.h"
#include "parameter_list.h"

OscillatorAGroup::OscillatorAGroup(ParameterGroupSet* parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Osc A", vg }, "Oscillator A", "Oscillator A", "Oscillator A")
{
}
