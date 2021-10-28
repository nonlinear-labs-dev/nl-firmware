#include "HardwareSourcesGroup.h"
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/RibbonParameter.h>
#include <parameters/PedalParameter.h>
#include <parameters/PitchbendParameter.h>
#include <parameters/AftertouchParameter.h>
#include <parameters/HardwareSourceSendParameter.h>

HardwareSourcesGroup::HardwareSourcesGroup(ParameterGroupSet* parent, OptRefSettings anOptional)
    : ParameterGroup(parent, { "CS", VoiceGroup::Global }, "HW Source", "Hardware Source", "Hardware Source")
    , m_settings(anOptional)
{
}

HardwareSourcesGroup::~HardwareSourcesGroup()
{
}

void HardwareSourcesGroup::init()
{
  auto pedal1 = appendParameter(new PedalParameter(this, getPedal1ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(this, pedal1, getPedal1SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000,
                                                  m_settings));

  auto pedal2 = appendParameter(new PedalParameter(this, getPedal2ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(this, pedal2, getPedal2SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000,
                                                  m_settings));

  auto pedal3 = appendParameter(new PedalParameter(this, getPedal3ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(this, pedal3, getPedal3SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000,
                                                  m_settings));

  auto pedal4 = appendParameter(new PedalParameter(this, getPedal4ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(this, pedal4, getPedal4SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000,
                                                  m_settings));

  auto bender = appendParameter(new PitchbendParameter(this, getPitchbendParameterID(),
                                         ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(this, bender, getBenderSendID(),
                                                  ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100,
                                                  1000, m_settings));

  auto at = appendParameter(new AftertouchParameter(this, getAftertouchParameterID(),
                                          ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(this, at, getAftertouchSendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000,
                                                  m_settings));

  auto ribbon1 = appendParameter(new RibbonParameter(this, getUpperRibbonParameterID(),
                                      ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(this, ribbon1, getRibbon1SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000,
                                                  m_settings));

  auto ribbon2 = appendParameter(new RibbonParameter(this, getLowerRibbonParameterID(),
                                      ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(this, ribbon2, getRibbon2SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000,
                                                  m_settings));
}

HardwareSourcesGroup::tPhysicalControlParameters HardwareSourcesGroup::getPhysicalControlParameters()
{
  tPhysicalControlParameters ret;

  for(auto param : getParameters())
    if(auto physicalParam = dynamic_cast<PhysicalControlParameter*>(param))
      if(dynamic_cast<HardwareSourceSendParameter*>(physicalParam) == nullptr) //to keep outside unchanged for now
        ret.push_back(physicalParam);

  return ret;
}
