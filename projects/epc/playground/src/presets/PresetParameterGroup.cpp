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

PresetParameter *PresetParameterGroup::findParameterByNumber(uint16_t num) const
{
  for(auto &a : m_parameters)
    if(a->getID().getNumber() == num)
      return a.get();
  return nullptr;
}

VoiceGroup PresetParameterGroup::getVoiceGroup() const
{
  return m_voiceGroup;
}

template <typename Other> void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const Other *other)
{
  for(auto &otherParam : other->getParameters())
  {
    ParameterId paramId = otherParam->getID();

    if(auto old = findParameterByNumber(paramId.getNumber()))
    {
      old->copyFrom(transaction, otherParam);
    }
    else
    {
      auto swap = UNDO::createSwapData(std::make_unique<PresetParameter>(*otherParam));

      auto doAndRedo = [=](auto) {
        std::unique_ptr<PresetParameter> s;
        swap->swapWith(s);
        m_parameters.push_back(std::move(s));
      };
      auto undo = [=](auto) {
        auto &place = m_parameters.back();
        swap->swapWith(place);
        m_parameters.pop_back();
      };

      transaction->addSimpleCommand(doAndRedo, undo);
    }
  }

  transaction->addUndoSwap(m_voiceGroup, other->getVoiceGroup());
}

template void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other);
template void PresetParameterGroup::copyFrom(UNDO::Transaction *transaction, const ::ParameterGroup *other);

void PresetParameterGroup::assignVoiceGroup(UNDO::Transaction *transaction, VoiceGroup vg)
{
  transaction->addSimpleCommand([this, swap = UNDO::createSwapData(vg)](auto) {
    swap->swapWith(m_voiceGroup);

    for(auto &g : m_parameters)
      g->assignVoiceGroup(m_voiceGroup);
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
  {
    auto& db = Application::get().getPresetManager()->getEditBuffer()->getParameterDB();
    name = db.getLongGroupName(m_parameters.front()->getID()).value_or(name);
  }

  writer.writeTag("group", Attribute("name", name), Attribute("afound", "true"), Attribute("bfound", "true"), [&] {
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
        auto& db = Application::get().getPresetManager()->getEditBuffer()->getParameterDB();
        auto paramName = db.getLongName(parameter->getID());
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

void PresetParameterGroup::TEST_deleteParameter(const ParameterId &id)
{
  m_parameters.erase(std::find_if(m_parameters.begin(), m_parameters.end(), [&](auto &p) { return p->getID() == id; }));
}
