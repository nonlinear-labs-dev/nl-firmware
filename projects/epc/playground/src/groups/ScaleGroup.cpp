#include <groups/ScaleGroup.h>
#include <parameters/ScaleParameter.h>
#include <parameters/scale-converters/BaseKeyScaleConverter.h>
#include <parameters/scale-converters/KeyScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include "parameter_declarations.h"
#include <Application.h>

ScaleGroup::ScaleGroup(ParameterGroupSet* parent)
    : ParameterGroup(parent, { "Scale", VoiceGroup::Global }, "Scale", "Scale", "Scale")
    , m_updateNames(Application::get().getMainContext(), std::chrono::milliseconds(200))
{
}

ScaleGroup::~ScaleGroup()
{
}

bool ScaleGroup::isAnyOffsetChanged() const
{
  for(const auto p : getParameters())
    if(p->getID().getNumber() != getScaleBaseParameterNumber())
      if(p->getControlPositionValue() != p->getDefaultValue())
        return true;

  return false;
}

int ScaleGroup::getScaleBaseParameterNumber()
{
  return C15::PID::Scale_Base_Key;
}

void ScaleGroup::init()
{
  auto baseKeyParam = new BaseScaleParameter(this, { getScaleBaseParameterNumber(), VoiceGroup::Global },
                                             ScaleConverter::get<BaseKeyScaleConverter>());

  baseKeyParam->onParameterChanged(sigc::mem_fun(this, &ScaleGroup::onBaseKeyParameterChanged), false);
  appendParameter(baseKeyParam);

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_0, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_1, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_2, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_3, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_4, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_5, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_6, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_7, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_8, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_9, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_10, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));

  appendParameter(new ScaleParameter(this, { C15::PID::Scale_Offset_11, VoiceGroup::Global },
                                     ScaleConverter::get<KeyScaleConverter>()));
}

void ScaleGroup::onBaseKeyParameterChanged(const Parameter*)
{
  m_updateNames.doTask([=]() {
    for(auto a : getParameters())
      a->onChange();
  });
}

bool ScaleGroup::isScaleParameter(const ParameterId& id)
{
  auto number = id.getNumber();
  return (number >= C15::PID::Scale_Base_Key && number <= C15::PID::Scale_Offset_11)
      || number == C15::PID::Scale_Offset_0;
}

bool ScaleGroup::isScaleParameter(const Parameter* parameter)
{
  if(parameter)
    return isScaleParameter(parameter->getID());
  return false;
}
