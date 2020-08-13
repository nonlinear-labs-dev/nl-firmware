#include "PresetParameterGroup.h"
#include <presets/Preset.h>
#include "PresetParameter.h"
#include <groups/ParameterGroup.h>
#include <xml/Writer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <xml/Attribute.h>

PresetParameterGroup::PresetParameterGroup(VoiceGroup vg)
    : m_voiceGroup { vg }
{
}

PresetParameterGroup::PresetParameterGroup(const ::ParameterGroup &other)
{
  for(auto &g : other.getParameters())
    m_parameters[g->getID()] = std::make_unique<PresetParameter>(*g);

  m_voiceGroup = other.getVoiceGroup();
}

PresetParameterGroup::PresetParameterGroup(const PresetParameterGroup &other)
{
  for(auto &g : other.m_parameters)
    m_parameters[g.first] = std::make_unique<PresetParameter>(*g.second);

  m_voiceGroup = other.getVoiceGroup();
}

PresetParameterGroup::~PresetParameterGroup() = default;

PresetParameter *PresetParameterGroup::findParameterByID(ParameterId id) const
{
  auto it = m_parameters.find(id);
  if(it != m_parameters.end())
    return it->second.get();
  return nullptr;
}

VoiceGroup PresetParameterGroup::getVoiceGroup() const
{
  return m_voiceGroup;
}

void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other)
{
  for(auto &g : m_parameters)
    if(auto o = other->findParameterByID(g.first))
      g.second->copyFrom(transaction, o);

  transaction->addUndoSwap(m_voiceGroup, other->getVoiceGroup());
}

void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const ::ParameterGroup *other)
{
  for(auto &g : m_parameters)
    if(auto o = other->getParameterByID(g.first))
      g.second->copyFrom(transaction, o);

  transaction->addUndoSwap(m_voiceGroup, other->getVoiceGroup());
}

void PresetParameterGroup::writeDiff(Writer &writer, const GroupId &groupId, const PresetParameterGroup *other) const
{
  if(!other)
  {
    writer.writeTag("group", Attribute("name", groupId.getName()), Attribute("afound", "true"),
                    Attribute("bfound", "false"), [&] {});
    return;
  }

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto group = eb->getParameterGroupByID(groupId);
  auto name = group->getLongName();

  writer.writeTag("group", Attribute("name", name), Attribute("afound", "true"), Attribute("bfound", "true"), [&] {
    auto numP = m_parameters.size();

    for(auto &parameter : m_parameters)
    {
      auto otherParameter = other->findParameterByID({ parameter.first.getNumber(), other->getVoiceGroup() });
      parameter.second->writeDiff(writer, parameter.first, otherParameter);
    }
  });
}

void PresetParameterGroup::writeDocument(Writer &writer) const
{
  for(auto &pair : m_parameters)
  {
    const auto param = static_cast<const PresetParameter *>(pair.second.get());
    param->writeDocument(writer);
  }
}
