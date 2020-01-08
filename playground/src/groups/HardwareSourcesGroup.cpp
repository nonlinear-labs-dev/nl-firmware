#include "HardwareSourcesGroup.h"
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/RibbonParameter.h>
#include <parameters/PedalParameter.h>
#include <parameters/PitchbendParameter.h>
#include <parameters/AftertouchParameter.h>

HardwareSourcesGroup::HardwareSourcesGroup(ParameterDualGroupSet* parent)
    : ParameterGroup(parent, { "CS", VoiceGroup::Global }, "HW Source", "Hardware Source", "Hardware Source")
{
}

HardwareSourcesGroup::~HardwareSourcesGroup()
{
}

void HardwareSourcesGroup::init()
{
  appendParameter(new PedalParameter(this, getPedal1ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new PedalParameter(this, getPedal2ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new PedalParameter(this, getPedal3ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new PedalParameter(this, getPedal4ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new PitchbendParameter(this, getPitchbendParameterID(),
                                         ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new AftertouchParameter(this, getAftertouchParameterID(),
                                          ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new RibbonParameter(this, getUpperRibbonParameterID(),
                                      ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new RibbonParameter(this, getLowerRibbonParameterID(),
                                      ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));
}

HardwareSourcesGroup::tPhysicalControlParameters HardwareSourcesGroup::getPhysicalControlParameters()
{
  tPhysicalControlParameters ret;

  for(auto param : getParameters())
    if(auto physicalParam = dynamic_cast<PhysicalControlParameter*>(param))
      ret.push_back(physicalParam);

  return ret;
}

bool HardwareSourcesGroup::isHardwareSourceParameter(const Parameter* parameter)
{
  if(parameter)
    return isHardwareSourceParameter(parameter->getID());
  return false;
}

bool HardwareSourcesGroup::isHardwareSourceParameter(const ParameterId& id)
{
  const auto isRibbon = getLowerRibbonParameterID() == id || getUpperRibbonParameterID() == id;
  const auto isBender = getPitchbendParameterID() == id;
  const auto isPedal = getPedal1ParameterID() == id || getPedal2ParameterID() == id || getPedal3ParameterID() == id
      || getPedal4ParameterID() == id;
  const auto isAfter = getAftertouchParameterID() == id;
  return isRibbon || isBender || isPedal || isAfter;
}
