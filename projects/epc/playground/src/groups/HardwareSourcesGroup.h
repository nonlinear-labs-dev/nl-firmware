#pragma once

#include "ParameterGroup.h"
#include <parameter_descriptor.h>
#include <parameters/ParameterTypes.h>
#include <parameters/ParameterTypes.h>

class ModulationRoutingParameter;
class PhysicalControlParameter;
class MacroControlParameter;

class HardwareSourcesGroup : public ParameterGroup
{
 public:
  explicit HardwareSourcesGroup(ParameterGroupSet* parent, OptRefSettings anOptional);
  ~HardwareSourcesGroup() override;

  typedef PhysicalControlParameter* tPhysicalControlParameter;
  typedef std::list<tPhysicalControlParameter> tPhysicalControlParameters;
  tPhysicalControlParameters getPhysicalControlParameters();

  static ParameterId getUpperRibbonParameterID()
  {
    return { C15::PID::Ribbon_1, VoiceGroup::Global };
  }

  static ParameterId getLowerRibbonParameterID()
  {
    return { C15::PID::Ribbon_2, VoiceGroup::Global };
  }

  static ParameterId getPedal1ParameterID()
  {
    return { C15::PID::Pedal_1, VoiceGroup::Global };
  }

  static ParameterId getPedal2ParameterID()
  {
    return { C15::PID::Pedal_2, VoiceGroup::Global };
  }

  static ParameterId getPedal3ParameterID()
  {
    return { C15::PID::Pedal_3, VoiceGroup::Global };
  }

  static ParameterId getPedal4ParameterID()
  {
    return { C15::PID::Pedal_4, VoiceGroup::Global };
  }

  static ParameterId getPitchbendParameterID()
  {
    return { C15::PID::Bender, VoiceGroup::Global };
  }

  static ParameterId getAftertouchParameterID()
  {
    return { C15::PID::Aftertouch, VoiceGroup::Global };
  }

  static ParameterId getPedal1SendID()
  {
    return { static_cast<int>(PlaceholderIDS::Pedal1_Send), VoiceGroup::Global };
  }

  static ParameterId getPedal2SendID()
  {
    return { static_cast<int>(PlaceholderIDS::Pedal2_Send), VoiceGroup::Global };
  }

  static ParameterId getPedal3SendID()
  {
    return { static_cast<int>(PlaceholderIDS::Pedal3_Send), VoiceGroup::Global };
  }

  static ParameterId getPedal4SendID()
  {
    return { static_cast<int>(PlaceholderIDS::Pedal4_Send), VoiceGroup::Global };
  }

  static ParameterId getRibbon1SendID()
  {
    return { static_cast<int>(PlaceholderIDS::Ribbon1_Send), VoiceGroup::Global };
  }

  static ParameterId getRibbon2SendID()
  {
    return { static_cast<int>(PlaceholderIDS::Ribbon2_Send), VoiceGroup::Global };
  }

  static ParameterId getAftertouchSendID()
  {
    return { static_cast<int>(PlaceholderIDS::Aftertouch_Send), VoiceGroup::Global };
  }

  static ParameterId getBenderSendID()
  {
    return { static_cast<int>(PlaceholderIDS::Bender_Send), VoiceGroup::Global };
  }

  void init() override;

 private:
  OptRefSettings m_settings;
};
