#include "PresetParameterGroupSerializer.h"
#include "PresetParameterSerializer.h"
#include <presets/PresetParameterGroup.h>
#include <presets/PresetParameter.h>
#include <parameter_declarations.h>
#include <nltools/logging/Log.h>

// change this signature and following serializers to accept a list of groups/parameters
// read can then directly fill all requested parameters with the values read -> split
// write will still create the group serializer with only one argument

PresetParameterGroupSerializer::PresetParameterGroupSerializer(PresetParameterGroup *paramGroup, SoundType type)
    : Serializer(getTagName())
    , m_paramGroup(paramGroup)
    , m_type(type)
{
}

PresetParameterGroupSerializer::~PresetParameterGroupSerializer()
{
}

Glib::ustring PresetParameterGroupSerializer::getTagName()
{
  return "parameter-group";
}

void PresetParameterGroupSerializer::writeTagContent(Writer &writer) const
{
  if(m_paramGroup)
    for(auto &param : m_paramGroup->m_parameters)
    {
      PresetParameterSerializer paramSerializer(param.get(), m_type);
      paramSerializer.write(writer, Attribute("id", param->getID()));
    }
}

void PresetParameterGroupSerializer::readTagContent(Reader &reader) const
{
  if(m_paramGroup)
  {
    reader.onTag(PresetParameterSerializer::getTagName(), [&](const Attributes &attr) mutable {
      ParameterId id(attr.get("id"));

      //Prevent old ParameterID to slip through
      if(id == ParameterId{C15::PID::Split_Split_Point, VoiceGroup::Global})
      {
        id = {C15::PID::Split_Split_Point, VoiceGroup::I};
      }

      if(auto p = m_paramGroup->findParameterByID(id))
        return new PresetParameterSerializer(p, m_type);

      nltools::Log::error("creating param:", id);
      auto param = std::make_unique<PresetParameter>(id);
      auto ret = new PresetParameterSerializer(param.get(), m_type);
      m_paramGroup->m_parameters.push_back(std::move(param));
      return ret;
    });
  }
}
