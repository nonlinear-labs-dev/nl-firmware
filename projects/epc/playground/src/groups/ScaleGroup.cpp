#include <groups/ScaleGroup.h>
#include <parameters/ScaleParameter.h>
#include "parameter_declarations.h"
#include <Application.h>

ScaleGroup::ScaleGroup(ParameterGroupSet* parent)
    : ParameterGroup(parent, { "Scale", VoiceGroup::Global }, "Scale", "Scale", "Scale")
    , m_updateNames(Application::get().getMainContext(), std::chrono::milliseconds(200))
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
  ParameterGroup::init();
  auto baseKeyParam = findParameterByID({ getScaleBaseParameterNumber(), VoiceGroup::Global });
  baseKeyParam->onParameterChanged(sigc::mem_fun(this, &ScaleGroup::onBaseKeyParameterChanged), false);
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

bool ScaleGroup::isScaleOffsetParameter(int id)
{
  return (id >= C15::PID::Scale_Offset_1 && id <= C15::PID::Scale_Offset_11) || id == C15::PID::Scale_Offset_0;
}

bool ScaleGroup::isScaleParameter(const Parameter* parameter)
{
  if(parameter)
    return isScaleParameter(parameter->getID());
  return false;
}
