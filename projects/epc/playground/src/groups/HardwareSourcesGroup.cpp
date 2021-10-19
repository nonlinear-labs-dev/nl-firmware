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

ParameterId getPedal1SendID()
{
  return { static_cast<int>(PlaceholderIDS::Pedal1_Send), VoiceGroup::Global };
}

ParameterId getPedal2SendID()
{
  return { static_cast<int>(PlaceholderIDS::Pedal2_Send), VoiceGroup::Global };
}

ParameterId getPedal3SendID()
{
  return { static_cast<int>(PlaceholderIDS::Pedal3_Send), VoiceGroup::Global };
}

ParameterId getPedal4SendID()
{
  return { static_cast<int>(PlaceholderIDS::Pedal4_Send), VoiceGroup::Global };
}

ParameterId getRibbon1SendID()
{
  return { static_cast<int>(PlaceholderIDS::Ribbon1_Send), VoiceGroup::Global };
}

ParameterId getRibbon2SendID()
{
  return { static_cast<int>(PlaceholderIDS::Ribbon2_Send), VoiceGroup::Global };
}

ParameterId getAftertouchSendID()
{
  return { static_cast<int>(PlaceholderIDS::Aftertouch_Send), VoiceGroup::Global };
}

ParameterId getBenderSendID()
{
  return { static_cast<int>(PlaceholderIDS::Bender_Send), VoiceGroup::Global };
}

void HardwareSourcesGroup::init()
{
  appendParameter(new PedalParameter(this, getPedal1ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(
      this, getPedal1SendID(), ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000, m_settings));

  appendParameter(new PedalParameter(this, getPedal2ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(
      this, getPedal2SendID(), ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000, m_settings));

  appendParameter(new PedalParameter(this, getPedal3ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(
      this, getPedal3SendID(), ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000, m_settings));

  appendParameter(new PedalParameter(this, getPedal4ParameterID(),
                                     ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(
      this, getPedal4SendID(), ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000, m_settings));

  appendParameter(new PitchbendParameter(this, getPitchbendParameterID(),
                                         ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(
      this, getBenderSendID(), ScaleConverter::get<LinearBipolar100PercentScaleConverter>(), 0, 100, 1000, m_settings));

  appendParameter(new AftertouchParameter(this, getAftertouchParameterID(),
                                          ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(
      this, getAftertouchSendID(), ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000, m_settings));

  appendParameter(new RibbonParameter(this, getUpperRibbonParameterID(),
                                      ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(
      this, getRibbon1SendID(), ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000, m_settings));

  appendParameter(new RibbonParameter(this, getLowerRibbonParameterID(),
                                      ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000));

  appendParameter(new HardwareSourceSendParameter(
      this, getRibbon2SendID(), ScaleConverter::get<Linear100PercentScaleConverter>(), 0.5, 100, 1000, m_settings));
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
