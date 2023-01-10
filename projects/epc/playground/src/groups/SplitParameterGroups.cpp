#include <parameters/SplitPointParameter.h>
#include <parameter_declarations.h>
#include "SplitParameterGroups.h"

SplitParameterGroups::SplitParameterGroups(ParameterGroupSet *parent, VoiceGroup vg)
    : ParameterGroup(parent, { "Split", vg }, "Split", "Split", "Split")
{
}

void SplitParameterGroups::init()
{
  appendParameter(new SplitPointParameter(this, { C15::PID::Split_Split_Point, getVoiceGroup() }));
}

bool SplitParameterGroups::isSplitPoint(const Parameter *p)
{
  return isSplitPoint(p->getID());
}

bool SplitParameterGroups::isSplitPoint(const ParameterId &id)
{
  return id.getNumber() == C15::PID::Split_Split_Point;
}
