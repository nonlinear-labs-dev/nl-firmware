#include <Application.h>
#include "EditBuffer.h"
#include "PresetParameterGroups.h"
#include "PresetManager.h"
#include "Preset.h"

PresetParameterGroups::PresetParameterGroups(UpdateDocumentContributor *parent)
    : AttributesOwner(parent)
{
}

PresetParameterGroups::PresetParameterGroups(UpdateDocumentContributor *parent, const Preset &other)
    : PresetParameterGroups(parent)
{
  init(other);
}

PresetParameterGroups::PresetParameterGroups(UpdateDocumentContributor *parent, const EditBuffer &editbuffer)
    : PresetParameterGroups(parent)
{
  init(editbuffer);
}

void PresetParameterGroups::writeDocument(Writer &writer, tUpdateID knownRevision) const
{
  for(auto &pair : m_parameterGroups)
    pair.second->writeDocument(writer);

  AttributesOwner::writeDocument(writer, knownRevision);
}

PresetParameter *PresetParameterGroups::findParameterByID(int id)
{
  return const_cast<PresetParameter *>(const_cast<const PresetParameterGroups *>(this)->findParameterByID(id));
}

const PresetParameter *PresetParameterGroups::findParameterByID(int id) const
{
  for(auto &pair : m_parameterGroups)
  {
    if(auto param = pair.second->findParameterByID(id))
      return param;
  }
  return nullptr;
}

void PresetParameterGroups::init(const EditBuffer &editbuffer)
{
  for(auto &g : editbuffer.getParameterGroups())
    m_parameterGroups[g->getID()] = std::make_unique<PresetParameterGroup>(*g);
}

void PresetParameterGroups::init(const Preset &preset)
{
  std::for_each(preset.m_parameterGroups.begin(), preset.m_parameterGroups.end(), [this](auto &a) {
    m_parameterGroups[a.first] = std::make_unique<PresetParameterGroup>(*a.second.get());
  });
}

void PresetParameterGroups::copyParamSet(UNDO::Transaction *transaction, const Preset *other)
{
  for(auto &pair : m_parameterGroups)
  {
    std::unique_ptr<PresetParameterGroup> &group = pair.second;
    auto& otherGroup = other->m_parameterGroups.at(pair.first);
    group->copyFrom(transaction, otherGroup.get());
  }
}
