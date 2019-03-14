#include "PresetParameterGroups.h"
#include "Preset.h"

PresetParameterGroups::PresetParameterGroups(UpdateDocumentContributor *parent) : AttributesOwner(parent) {
}

PresetParameterGroups::PresetParameterGroups(UpdateDocumentContributor* parent, const Preset &other) : PresetParameterGroups(parent) {
    std::for_each(other.m_parameterGroups.begin(), other.m_parameterGroups.end(), [this](auto &a) {
        m_parameterGroups[a.first] = std::make_unique<PresetParameterGroup>(*a.second.get());
    });
}

PresetParameterGroups::PresetParameterGroups(UpdateDocumentContributor* parent, const EditBuffer& editbuffer) : PresetParameterGroups(parent) {
    for(auto &g : editbuffer.getParameterGroups())
        m_parameterGroups[g->getID()] = std::make_unique<PresetParameterGroup>(*g);
}

void PresetParameterGroups::writeDocument(Writer &writer, tUpdateID knownRevision) const {
    for(auto& [id, group]: m_parameterGroups) {
        group->writeDocument(writer);
    }
    AttributesOwner::writeDocument(writer, knownRevision);
}
