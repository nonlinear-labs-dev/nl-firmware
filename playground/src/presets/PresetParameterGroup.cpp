#include "PresetParameterGroup.h"
#include <presets/Preset.h>
#include "PresetParameter.h"
#include <groups/ParameterGroup.h>
#include <xml/Writer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

PresetParameterGroup::PresetParameterGroup(Preset *parent)
    : UpdateDocumentContributor(parent)
{
}

PresetParameterGroup::PresetParameterGroup(Preset *parent, const ::ParameterGroup &other)
    : PresetParameterGroup(parent)
{
  for(auto &g : other.getParameters())
    m_parameters[g->getID()] = std::make_unique<PresetParameter>(this, *g);
}

PresetParameterGroup::PresetParameterGroup(Preset *parent, const PresetParameterGroup &other)
    : PresetParameterGroup(parent)
{
  for(auto &g : other.m_parameters)
    m_parameters[g.first] = std::make_unique<PresetParameter>(this, *g.second);
}

PresetParameterGroup::~PresetParameterGroup() = default;

PresetParameter *PresetParameterGroup::findParameterByID(int id) const
{
  auto it = m_parameters.find(id);
  if(it != m_parameters.end())
    return it->second.get();
  return nullptr;
}

void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other)
{
  for(auto &g : m_parameters)
    if(auto o = other->findParameterByID(g.first))
      g.second->copyFrom(transaction, o);
}

void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const ::ParameterGroup *other)
{
  for(auto &g : m_parameters)
    if(auto o = other->getParameterByID(g.first))
      g.second->copyFrom(transaction, o);
}

void PresetParameterGroup::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
}

void PresetParameterGroup::writeDiff(Writer &writer, const std::string &groupId,
                                     const PresetParameterGroup *other) const
{
  auto name = Application::get().getPresetManager()->getEditBuffer()->getParameterGroupByID(groupId)->getLongName();

  writer.writeTag("group", Attribute("name", name), [&] {
    for(auto &parameter : m_parameters)
    {
      auto otherParameter = other->findParameterByID(parameter.first);
      parameter.second->writeDiff(writer, parameter.first, otherParameter);
    }
  });
}
