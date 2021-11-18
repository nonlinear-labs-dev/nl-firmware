#include "PresetParameterGroup.h"
#include <presets/Preset.h>
#include <parameters/names/ParameterDB.h>
#include "PresetParameter.h"
#include <groups/ParameterGroup.h>
#include <xml/Writer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <xml/Attribute.h>

PresetParameterGroup::PresetParameterGroup(const ::ParameterGroup &other)
{
  for(auto &g : other.getParameters())
    m_parameters.push_back(std::make_unique<PresetParameter>(*g));

  m_voiceGroup = other.getVoiceGroup();
}

PresetParameterGroup::PresetParameterGroup(const PresetParameterGroup &other)
{
  for(auto &g : other.m_parameters)
    m_parameters.push_back(std::make_unique<PresetParameter>(*g));

  m_voiceGroup = other.getVoiceGroup();
}

PresetParameterGroup::~PresetParameterGroup() = default;

PresetParameter *PresetParameterGroup::findParameterByID(ParameterId id) const
{
  for(auto &a : m_parameters)
    if(a->getID() == id)
      return a.get();
  return nullptr;
}

VoiceGroup PresetParameterGroup::getVoiceGroup() const
{
  return m_voiceGroup;
}

void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other)
{
  for(auto &g : m_parameters)
    if(auto o = other->findParameterByID(g->getID()))
      g->copyFrom(transaction, o);

  transaction->addUndoSwap(m_voiceGroup, other->getVoiceGroup());
}

void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const ::ParameterGroup *other)
{
  for(auto &g : m_parameters)
    if(auto o = other->getParameterByID(g->getID()))
      g->copyFrom(transaction, o);

  transaction->addUndoSwap(m_voiceGroup, other->getVoiceGroup());
}

void PresetParameterGroup::assignVoiceGroup(UNDO::Transaction *transaction, VoiceGroup vg)
{
  transaction->addUndoSwap(m_voiceGroup, vg);

  transaction->addSimpleCommand(
      [=](auto)
      {
        for(auto &g : m_parameters)
          g->assignVoiceGroup(vg);
      });
}

void PresetParameterGroup::writeDiff(Writer &writer, const GroupId &groupId, const PresetParameterGroup *other) const
{
  if(!other)
  {
    writer.writeTag("group", Attribute("name", groupId.getName()), Attribute("afound", "true"),
                    Attribute("bfound", "false"), [&] {});
    return;
  }

  std::string name = groupId.getName();

  if(!m_parameters.empty())
    name = ParameterDB::get().getLongGroupName(m_parameters.front()->getID()).value_or(name);

  writer.writeTag(
      "group", Attribute("name", name), Attribute("afound", "true"), Attribute("bfound", "true"),
      [&]
      {
        std::vector<int> writtenParameters;

        for(auto &parameter : m_parameters)
        {
          auto otherParameter = other->findParameterByID({ parameter->getID().getNumber(), other->getVoiceGroup() });
          parameter->writeDiff(writer, parameter->getID(), otherParameter);
          writtenParameters.emplace_back(parameter->getID().getNumber());
        }

        for(auto &parameter : other->getParameters())
        {
          if(std::find(writtenParameters.begin(), writtenParameters.end(), parameter->getID().getNumber())
             == writtenParameters.end())
          {
            auto paramName = ParameterDB::get().getLongName(parameter->getID());
            writer.writeTag("parameter", Attribute("name", paramName), Attribute("afound", "false"),
                            Attribute("bfound", "true"), [] {});
          }
        }
      });
}

void PresetParameterGroup::writeDocument(Writer &writer) const
{
  for(auto &p : m_parameters)
  {
    const auto param = static_cast<const PresetParameter *>(p.get());
    param->writeDocument(writer);
  }
}
