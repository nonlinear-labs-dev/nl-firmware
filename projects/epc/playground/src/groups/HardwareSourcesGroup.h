#pragma once

#include "ParameterGroup.h"
#include <parameter_descriptor.h>

class ModulationRoutingParameter;
class PhysicalControlParameter;
class MacroControlParameter;
class HardwareSourceSendParameter;
class Settings;

class HardwareSourcesGroup : public ParameterGroup
{
 public:
  explicit HardwareSourcesGroup(ParameterGroupSet* parent, Settings* settings);
  ~HardwareSourcesGroup() override;

  typedef PhysicalControlParameter* tPhysicalControlParameter;
  typedef std::list<tPhysicalControlParameter> tPhysicalControlParameters;
  tPhysicalControlParameters getPhysicalControlParameters();

  typedef HardwareSourceSendParameter* tSendParameter;
  typedef std::list<tSendParameter> tSendParameters;
  tSendParameters getSendParameters();

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
    return { static_cast<int>(C15::PID::Pedal_1_Send), VoiceGroup::Global };
  }

  static ParameterId getPedal2SendID()
  {
    return { static_cast<int>(C15::PID::Pedal_2_Send), VoiceGroup::Global };
  }

  static ParameterId getPedal3SendID()
  {
    return { static_cast<int>(C15::PID::Pedal_3_Send), VoiceGroup::Global };
  }

  static ParameterId getPedal4SendID()
  {
    return { static_cast<int>(C15::PID::Pedal_4_Send), VoiceGroup::Global };
  }

  static ParameterId getRibbon1SendID()
  {
    return { static_cast<int>(C15::PID::Ribbon_1_Send), VoiceGroup::Global };
  }

  static ParameterId getRibbon2SendID()
  {
    return { static_cast<int>(C15::PID::Ribbon_2_Send), VoiceGroup::Global };
  }

  static ParameterId getAftertouchSendID()
  {
    return { static_cast<int>(C15::PID::Aftertouch_Send), VoiceGroup::Global };
  }

  static ParameterId getBenderSendID()
  {
    return { static_cast<int>(C15::PID::Bender_Send), VoiceGroup::Global };
  }

  void init() override;

 private:
  Settings* m_settings;
};
