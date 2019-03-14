#pragma once

#include <http/UpdateDocumentContributor.h>
#include "AttributesOwner.h"
#include "PresetParameterGroup.h"
#include "EditBuffer.h"

class Preset;

class PresetParameterGroups : public AttributesOwner
{
 public:
  PresetParameterGroups(UpdateDocumentContributor* parent);
  PresetParameterGroups(UpdateDocumentContributor* parent, const Preset& other);
  PresetParameterGroups(UpdateDocumentContributor* parent, const EditBuffer& editbuffer);

  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;

 protected:
  using GroupPtr = std::unique_ptr<PresetParameterGroup>;
  std::unordered_map<std::string, GroupPtr> m_parameterGroups;
};