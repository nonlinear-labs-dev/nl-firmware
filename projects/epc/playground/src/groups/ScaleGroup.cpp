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