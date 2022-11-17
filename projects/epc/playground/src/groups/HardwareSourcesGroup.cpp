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
}

void HardwareSourcesGroup::init()
{
  auto pedal1 = appendParameter(new PedalParameter(this, getPedal1ParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *pedal1, getPedal1SendID(), m_settings));
  auto pedal2 = appendParameter(new PedalParameter(this, getPedal2ParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *pedal2, getPedal2SendID(), m_settings));
  auto pedal3 = appendParameter(new PedalParameter(this, getPedal3ParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *pedal3, getPedal3SendID(), m_settings));
  auto pedal4 = appendParameter(new PedalParameter(this, getPedal4ParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *pedal4, getPedal4SendID(), m_settings));
  auto bender = appendParameter(new PitchbendParameter(this, getPitchbendParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *bender, getBenderSendID(), m_settings));
  auto at = appendParameter(new AftertouchParameter(this, getAftertouchParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *at, getAftertouchSendID(), m_settings));
  auto ribbon1 = appendParameter(new RibbonParameter(this, getUpperRibbon1ParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *ribbon1, getRibbon1SendID(), m_settings));
  auto ribbon2 = appendParameter(new RibbonParameter(this, getLowerRibbon2ParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *ribbon2, getRibbon2SendID(), m_settings));
  auto ribbon3 = appendParameter(new RibbonParameter(this, getUpperRibbon3ParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *ribbon3, getRibbon3SendID(), m_settings));
  auto ribbon4 = appendParameter(new RibbonParameter(this, getLowerRibbon4ParameterID()));
  appendParameter(new HardwareSourceSendParameter(this, *ribbon4, getRibbon4SendID(), m_settings));

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