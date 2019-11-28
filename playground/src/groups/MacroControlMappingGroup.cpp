#include "MacroControlMappingGroup.h"
#include "HardwareSourcesGroup.h"

#include "parameters/PhysicalControlParameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameters/ModulationRoutingParameter.h"
#include "parameters/MacroControlParameter.h"
#include "parameters/scale-converters/LinearBipolar100PercentScaleConverter.h"

MacroControlMappingGroup::MacroControlMappingGroup(ParameterDualGroupSet *parent, ParameterGroup *hw,
                                                   ParameterGroup *mc, VoiceGroup voicegroup)
    : ParameterGroup(parent, "MCM", "HW Amount", "Hardware Amount", "Hardware Amount", voicegroup)
    , m_mc(mc)
    , m_hw(hw)
{
}

MacroControlMappingGroup::~MacroControlMappingGroup()
{
}

void MacroControlMappingGroup::init()
{
  auto mc_a = dynamic_cast<MacroControlParameter *>(m_mc->getParameterByID({243, getVoiceGroup()}));
  auto mc_b = dynamic_cast<MacroControlParameter *>(m_mc->getParameterByID({244, getVoiceGroup()}));
  auto mc_c = dynamic_cast<MacroControlParameter *>(m_mc->getParameterByID({245, getVoiceGroup()}));
  auto mc_d = dynamic_cast<MacroControlParameter *>(m_mc->getParameterByID({246, getVoiceGroup()}));
  auto mc_e = dynamic_cast<MacroControlParameter *>(m_mc->getParameterByID({369, getVoiceGroup()}));
  auto mc_f = dynamic_cast<MacroControlParameter *>(m_mc->getParameterByID({370, getVoiceGroup()}));

  auto pedal1
      = dynamic_cast<PhysicalControlParameter *>(m_hw->getParameterByID(HardwareSourcesGroup::getPedal1ParameterID()));
  auto pedal2
      = dynamic_cast<PhysicalControlParameter *>(m_hw->getParameterByID(HardwareSourcesGroup::getPedal2ParameterID()));
  auto pedal3
      = dynamic_cast<PhysicalControlParameter *>(m_hw->getParameterByID(HardwareSourcesGroup::getPedal3ParameterID()));
  auto pedal4
      = dynamic_cast<PhysicalControlParameter *>(m_hw->getParameterByID(HardwareSourcesGroup::getPedal4ParameterID()));
  auto pitchbend = dynamic_cast<PhysicalControlParameter *>(
      m_hw->getParameterByID(HardwareSourcesGroup::getPitchbendParameterID()));
  auto aftertouch = dynamic_cast<PhysicalControlParameter *>(
      m_hw->getParameterByID(HardwareSourcesGroup::getAftertouchParameterID()));
  auto ribbon1 = dynamic_cast<PhysicalControlParameter *>(
      m_hw->getParameterByID(HardwareSourcesGroup::getUpperRibbonParameterID()));
  auto ribbon2 = dynamic_cast<PhysicalControlParameter *>(
      m_hw->getParameterByID(HardwareSourcesGroup::getLowerRibbonParameterID()));

  appendParameter(new ModulationRoutingParameter(this, { 255, getVoiceGroup() }, pedal1, mc_a,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 256, getVoiceGroup() }, pedal1, mc_b,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 257, getVoiceGroup() }, pedal1, mc_c,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 258, getVoiceGroup() }, pedal1, mc_d,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {373, getVoiceGroup()}, pedal1, mc_e,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {374, getVoiceGroup()}, pedal1, mc_f,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {260, getVoiceGroup()}, pedal2, mc_a,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 261, getVoiceGroup() }, pedal2, mc_b,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 262, getVoiceGroup() }, pedal2, mc_c,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 263, getVoiceGroup() }, pedal2, mc_d,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {375, getVoiceGroup()}, pedal2, mc_e,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {376, getVoiceGroup()}, pedal2, mc_f,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {265, getVoiceGroup()}, pedal3, mc_a,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 266, getVoiceGroup() }, pedal3, mc_b,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 267, getVoiceGroup() }, pedal3, mc_c,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 268, getVoiceGroup() }, pedal3, mc_d,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {377, getVoiceGroup()}, pedal3, mc_e,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {378, getVoiceGroup()}, pedal3, mc_f,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 270, getVoiceGroup() }, pedal4, mc_a,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 271, getVoiceGroup() }, pedal4, mc_b,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 272, getVoiceGroup() }, pedal4, mc_c,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 273, getVoiceGroup() }, pedal4, mc_d,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {379, getVoiceGroup()}, pedal4, mc_e,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {380, getVoiceGroup()}, pedal4, mc_f,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 275, getVoiceGroup() }, pitchbend, mc_a,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 276, getVoiceGroup() }, pitchbend, mc_b,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 277, getVoiceGroup() }, pitchbend, mc_c,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 278, getVoiceGroup() }, pitchbend, mc_d,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {381, getVoiceGroup()}, pitchbend, mc_e,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {382, getVoiceGroup()}, pitchbend, mc_f,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 280, getVoiceGroup() }, aftertouch, mc_a,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 281, getVoiceGroup() }, aftertouch, mc_b,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 282, getVoiceGroup() }, aftertouch, mc_c,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 283, getVoiceGroup() }, aftertouch, mc_d,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {383, getVoiceGroup()}, aftertouch, mc_e,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {384, getVoiceGroup()}, aftertouch, mc_f,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 285, getVoiceGroup() }, ribbon1, mc_a,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 286, getVoiceGroup() }, ribbon1, mc_b,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 287, getVoiceGroup() }, ribbon1, mc_c,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 288, getVoiceGroup() }, ribbon1, mc_d,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {385, getVoiceGroup()}, ribbon1, mc_e,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {386, getVoiceGroup()}, ribbon1, mc_f,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 290, getVoiceGroup() }, ribbon2, mc_a,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 291, getVoiceGroup() }, ribbon2, mc_b,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 292, getVoiceGroup() }, ribbon2, mc_c,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, { 293, getVoiceGroup() }, ribbon2, mc_d,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {387, getVoiceGroup()}, ribbon2, mc_e,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));

  appendParameter(new ModulationRoutingParameter(this, {388, getVoiceGroup()}, ribbon2, mc_f,
                                                 ScaleConverter::get<LinearBipolar100PercentScaleConverter>()));
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

const ModulationRoutingParameter *
    MacroControlMappingGroup::getModulationRoutingParameterFor(const PhysicalControlParameter *src,
                                                               const MacroControlParameter *mc)
{
  for(auto param : getParameters())
    if(auto routingParam = dynamic_cast<const ModulationRoutingParameter *>(param))
      if(src == routingParam->getSourceParameter() && mc == routingParam->getTargetParameter())
        return routingParam;

  return nullptr;
}

MacroControlMappingGroup::tPhysicalControlParameters MacroControlMappingGroup::getPhysicalControlParameters()
{
  tPhysicalControlParameters ret;

  for(auto param : getParameters())
    if(auto physicalParam = dynamic_cast<PhysicalControlParameter *>(param))
      ret.push_back(physicalParam);

  return ret;
}
