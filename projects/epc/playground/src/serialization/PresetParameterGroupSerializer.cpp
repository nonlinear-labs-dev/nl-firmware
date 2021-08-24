#include "PresetParameterGroupSerializer.h"
#include "PresetParameterSerializer.h"
#include <presets/PresetParameterGroup.h>
#include <presets/PresetParameter.h>
#include <parameter_declarations.h>
#include <nltools/logging/Log.h>
#include <utility>

// change this signature and following serializers to accept a list of groups/parameters
// read can then directly fill all requested parameters with the values read -> split
// write will still create the group serializer with only one argument

PresetParameterGroupSerializer::PresetParameterGroupSerializer(std::vector<PresetParameterGroup *> paramGroup,
                                                               SoundType type)
    : Serializer(getTagName())
    , m_paramGroups(std::move(paramGroup))
    , m_type(type)
{
}

PresetParameterGroupSerializer::~PresetParameterGroupSerializer() = default;

Glib::ustring PresetParameterGroupSerializer::getTagName()
{
  return "parameter-group";
}

void PresetParameterGroupSerializer::writeTagContent(Writer &writer) const
{
  for(auto &g : m_paramGroups)
    for(auto &param : g->m_parameters)
    {
      PresetParameterSerializer paramSerializer({ param.get() }, m_type);
      paramSerializer.write(writer, Attribute("id", param->getID()));
    }
}

void PresetParameterGroupSerializer::readTagContent(Reader &reader) const
{
  reader.onTag(PresetParameterSerializer::getTagName(),
               [&](const Attributes &attr) mutable
               {
                 std::vector<PresetParameter *> params;

                 for(auto &g : m_paramGroups)
                 {
                   for(auto &p : g->m_parameters)
                   {
                     params.emplace_back(p.get());
                   }
                 }

                 return new PresetParameterSerializer(params, m_type);
               });
}
