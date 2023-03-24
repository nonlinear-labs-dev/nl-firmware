#include "MacroControlMappingGroup.h"
#include "HardwareSourcesGroup.h"

#include "parameters/PhysicalControlParameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameters/ModulationRoutingParameter.h"
#include "parameters/MacroControlParameter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"
#include "parameter_group.h"

namespace
{
  auto getDesc()
  {
    return C15::ParameterGroups[static_cast<int>(C15::Descriptors::ParameterGroup::Mod_HA)];
  }

}

MacroControlMappingGroup::MacroControlMappingGroup(ParameterGroupSet *parent, ParameterGroup *hw, ParameterGroup *mc)
    : ParameterGroup(parent, getDesc(), VoiceGroup::Global)
    , m_mc(mc)
    , m_hw(hw)
{
}

void MacroControlMappingGroup::init()
{
  auto mc_a = m_mc->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
  auto mc_b = m_mc->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_B, VoiceGroup::Global });
  auto mc_c = m_mc->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_C, VoiceGroup::Global });
  auto mc_d = m_mc->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_D, VoiceGroup::Global });
  auto mc_e = m_mc->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_E, VoiceGroup::Global });
  auto mc_f = m_mc->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_F, VoiceGroup::Global });

  auto pedal1 = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getPedal1ParameterID());
  auto pedal2 = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getPedal2ParameterID());
  auto pedal3 = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getPedal3ParameterID());
  auto pedal4 = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getPedal4ParameterID());
  auto pitchbend
      = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getPitchbendParameterID());
  auto aftertouch
      = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getAftertouchParameterID());
  auto ribbon1
      = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getUpperRibbon1ParameterID());
  auto ribbon2
      = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getLowerRibbon2ParameterID());
  auto ribbon3
      = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getUpperRibbon3ParameterID());
  auto ribbon4
      = m_hw->findAndCastParameterByID<PhysicalControlParameter>(HardwareSourcesGroup::getLowerRibbon4ParameterID());

  using namespace C15::PID;
  appendParameter(new ModulationRoutingParameter(this, { Pedal_1_to_MC_A, VoiceGroup::Global }, pedal1, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_1_to_MC_B, VoiceGroup::Global }, pedal1, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_1_to_MC_C, VoiceGroup::Global }, pedal1, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_1_to_MC_D, VoiceGroup::Global }, pedal1, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_1_to_MC_E, VoiceGroup::Global }, pedal1, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_1_to_MC_F, VoiceGroup::Global }, pedal1, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_2_to_MC_A, VoiceGroup::Global }, pedal2, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_2_to_MC_B, VoiceGroup::Global }, pedal2, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_2_to_MC_C, VoiceGroup::Global }, pedal2, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_2_to_MC_D, VoiceGroup::Global }, pedal2, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_2_to_MC_E, VoiceGroup::Global }, pedal2, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_2_to_MC_F, VoiceGroup::Global }, pedal2, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_3_to_MC_A, VoiceGroup::Global }, pedal3, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_3_to_MC_B, VoiceGroup::Global }, pedal3, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_3_to_MC_C, VoiceGroup::Global }, pedal3, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_3_to_MC_D, VoiceGroup::Global }, pedal3, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_3_to_MC_E, VoiceGroup::Global }, pedal3, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_3_to_MC_F, VoiceGroup::Global }, pedal3, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_4_to_MC_A, VoiceGroup::Global }, pedal4, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_4_to_MC_B, VoiceGroup::Global }, pedal4, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_4_to_MC_C, VoiceGroup::Global }, pedal4, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_4_to_MC_D, VoiceGroup::Global }, pedal4, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_4_to_MC_E, VoiceGroup::Global }, pedal4, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Pedal_4_to_MC_F, VoiceGroup::Global }, pedal4, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Bender_to_MC_A, VoiceGroup::Global }, pitchbend, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Bender_to_MC_B, VoiceGroup::Global }, pitchbend, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Bender_to_MC_C, VoiceGroup::Global }, pitchbend, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Bender_to_MC_D, VoiceGroup::Global }, pitchbend, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Bender_to_MC_E, VoiceGroup::Global }, pitchbend, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Bender_to_MC_F, VoiceGroup::Global }, pitchbend, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Aftertouch_to_MC_A, VoiceGroup::Global }, aftertouch, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Aftertouch_to_MC_B, VoiceGroup::Global }, aftertouch, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Aftertouch_to_MC_C, VoiceGroup::Global }, aftertouch, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Aftertouch_to_MC_D, VoiceGroup::Global }, aftertouch, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Aftertouch_to_MC_E, VoiceGroup::Global }, aftertouch, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Aftertouch_to_MC_F, VoiceGroup::Global }, aftertouch, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_1_to_MC_A, VoiceGroup::Global }, ribbon1, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_1_to_MC_B, VoiceGroup::Global }, ribbon1, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_1_to_MC_C, VoiceGroup::Global }, ribbon1, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_1_to_MC_D, VoiceGroup::Global }, ribbon1, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_1_to_MC_E, VoiceGroup::Global }, ribbon1, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_1_to_MC_F, VoiceGroup::Global }, ribbon1, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_2_to_MC_A, VoiceGroup::Global }, ribbon2, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_2_to_MC_B, VoiceGroup::Global }, ribbon2, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_2_to_MC_C, VoiceGroup::Global }, ribbon2, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_2_to_MC_D, VoiceGroup::Global }, ribbon2, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_2_to_MC_E, VoiceGroup::Global }, ribbon2, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_2_to_MC_F, VoiceGroup::Global }, ribbon2, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_3_to_MC_A, VoiceGroup::Global }, ribbon3, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_3_to_MC_B, VoiceGroup::Global }, ribbon3, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_3_to_MC_C, VoiceGroup::Global }, ribbon3, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_3_to_MC_D, VoiceGroup::Global }, ribbon3, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_3_to_MC_E, VoiceGroup::Global }, ribbon3, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_3_to_MC_F, VoiceGroup::Global }, ribbon3, mc_f));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_4_to_MC_A, VoiceGroup::Global }, ribbon4, mc_a));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_4_to_MC_B, VoiceGroup::Global }, ribbon4, mc_b));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_4_to_MC_C, VoiceGroup::Global }, ribbon4, mc_c));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_4_to_MC_D, VoiceGroup::Global }, ribbon4, mc_d));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_4_to_MC_E, VoiceGroup::Global }, ribbon4, mc_e));
  appendParameter(new ModulationRoutingParameter(this, { Ribbon_4_to_MC_F, VoiceGroup::Global }, ribbon4, mc_f));
}

MacroControlMappingGroup::tModRoutingParams
    MacroControlMappingGroup::getModulationRoutingParametersFor(const MacroControlParameter *mc)
{
  tModRoutingParams ret;

  for(auto param : getParameters())
    if(auto routingParam = dynamic_cast<ModulationRoutingParameter *>(param))
      if(mc == routingParam->getTargetParameter())
        ret.push_back(routingParam);

  return ret;
}

MacroControlMappingGroup::tModRoutingParams
    MacroControlMappingGroup::getModulationRoutingParametersFor(const PhysicalControlParameter *p)
{
  tModRoutingParams ret;

  for(auto param : getParameters())
    if(auto routingParam = dynamic_cast<ModulationRoutingParameter *>(param))
      if(p == routingParam->getSourceParameter())
        ret.push_back(routingParam);

  return ret;
}

ModulationRoutingParameter *
    MacroControlMappingGroup::getModulationRoutingParameterFor(const PhysicalControlParameter *src,
                                                               const MacroControlParameter *mc)
{
  for(auto param : getParameters())
    if(auto routingParam = dynamic_cast<ModulationRoutingParameter *>(param))
      if(src == routingParam->getSourceParameter() && mc == routingParam->getTargetParameter())
        return routingParam;

  return nullptr;
}
