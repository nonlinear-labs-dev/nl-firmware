#include <Application.h>
#include "EditBuffer.h"
#include "PresetDualParameterGroups.h"
#include "PresetManager.h"
#include "Preset.h"

PresetDualParameterGroups::PresetDualParameterGroups(UpdateDocumentContributor *parent)
    : AttributesOwner(parent)
{
  m_type = SoundType::Single;
}

PresetDualParameterGroups::PresetDualParameterGroups(UpdateDocumentContributor *parent, const Preset &other)
    : AttributesOwner(parent, &other)
{
  init(&other);
}

PresetDualParameterGroups::PresetDualParameterGroups(UpdateDocumentContributor *parent, const EditBuffer &editbuffer)
    : AttributesOwner(parent, &editbuffer)
{
  m_type = editbuffer.getType();

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto &g : editbuffer.getParameterGroups(vg))
      m_parameterGroups[static_cast<int>(vg)][g->getID()] = std::make_unique<PresetParameterGroup>(*g);
}

void PresetDualParameterGroups::writeDocument(Writer &writer, tUpdateID knownRevision) const
{
#warning "TODO add second paramset to document!!"

  for(auto &pair : m_parameterGroups[static_cast<int>(
      Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection())])
    pair.second->writeDocument(writer);
  AttributesOwner::writeDocument(writer, knownRevision);
}

void PresetDualParameterGroups::init(const Preset *preset)
{
  m_type = preset->getType();

  for(auto vg : { VoiceGroup::I, VoiceGroup::II })
    for(auto &group : preset->m_parameterGroups[static_cast<int>(vg)])
      m_parameterGroups[static_cast<int>(vg)][group.first] = std::make_unique<PresetParameterGroup>(*group.second);
}
