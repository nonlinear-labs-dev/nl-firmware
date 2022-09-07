#include "HardwareSourcesGroup.h"
#include <parameters/scale-converters/Linear100PercentScaleConverter.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <parameters/RibbonParameter.h>
#include <parameters/PedalParameter.h>
#include <parameters/PitchbendParameter.h>
#include <parameters/AftertouchParameter.h>
#include <parameters/HardwareSourceSendParameter.h>
#include <device-settings/Settings.h>

HardwareSourcesGroup::HardwareSourcesGroup(ParameterGroupSet* parent, Settings* settings)
    : ParameterGroup(parent, { "CS", VoiceGroup::Global }, "HW Source", "Hardware Source", "Hardware Source")
    , m_settings(settings)
{
  auto pedal1 = appendParameter(
      new PedalParameter(this, getPedal1ParameterID(), ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new HardwareSourceSendParameter(this, *pedal1, getPedal1SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>()));

  auto pedal2 = appendParameter(
      new PedalParameter(this, getPedal2ParameterID(), ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new HardwareSourceSendParameter(this, *pedal2, getPedal2SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>()));

  auto pedal3 = appendParameter(
      new PedalParameter(this, getPedal3ParameterID(), ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new HardwareSourceSendParameter(this, *pedal3, getPedal3SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>()));

  auto pedal4 = appendParameter(
      new PedalParameter(this, getPedal4ParameterID(), ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new HardwareSourceSendParameter(this, *pedal4, getPedal4SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>()));

  auto bender = appendParameter(new PitchbendParameter(this, getPitchbendParameterID(),
                                                       ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new HardwareSourceSendParameter(this, *bender, getBenderSendID(),
                                                  ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  auto at = appendParameter(
      new AftertouchParameter(this, getAftertouchParameterID(), ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new HardwareSourceSendParameter(this, *at, getAftertouchSendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>()));

  auto ribbon1 = appendParameter(
      new RibbonParameter(this, getUpperRibbonParameterID(), ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new HardwareSourceSendParameter(this, *ribbon1, getRibbon1SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>()));

  auto ribbon2 = appendParameter(
      new RibbonParameter(this, getLowerRibbonParameterID(), ScaleConverter::get<Linear100PercentScaleConverter>()));

  appendParameter(new HardwareSourceSendParameter(this, *ribbon2, getRibbon2SendID(),
                                                  ScaleConverter::get<Linear100PercentScaleConverter>()));
}

HardwareSourcesGroup::~HardwareSourcesGroup()
{
}

void HardwareSourcesGroup::init()
{
  if(m_settings == nullptr)
    return;

  findAndCastParameterByID<PedalParameter>(getPedal1ParameterID())->init(*m_settings);
  findAndCastParameterByID<PedalParameter>(getPedal2ParameterID())->init(*m_settings);
  findAndCastParameterByID<PedalParameter>(getPedal3ParameterID())->init(*m_settings);
  findAndCastParameterByID<PedalParameter>(getPedal4ParameterID())->init(*m_settings);

  for(auto hwSend : getSendParameters())
  {
    hwSend->init(m_settings);
  }
}

HardwareSourcesGroup::tPhysicalControlParameters HardwareSourcesGroup::getPhysicalControlParameters()
{
  tPhysicalControlParameters ret;

  for(auto param : getParameters())
    if(auto physicalParam = dynamic_cast<PhysicalControlParameter*>(param))
      if(dynamic_cast<HardwareSourceSendParameter*>(physicalParam) == nullptr)  //to keep outside unchanged for now
        ret.push_back(physicalParam);

  return ret;
}

HardwareSourcesGroup::tSendParameters HardwareSourcesGroup::getSendParameters()
{
  tSendParameters ret;

  for(auto param : getParameters())
    if(auto sendParam = dynamic_cast<HardwareSourceSendParameter*>(param))
      ret.push_back(sendParam);

  return ret;
}