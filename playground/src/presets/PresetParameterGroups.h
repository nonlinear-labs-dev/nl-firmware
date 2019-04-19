#pragma once

#include <http/UpdateDocumentContributor.h>
#include "AttributesOwner.h"
#include "PresetParameterGroup.h"

class Preset;
class ParameterGroupSet;
class EditBuffer;

class PresetParameterGroups : public AttributesOwner
{
 public:
  PresetParameterGroups(UpdateDocumentContributor* parent);
  PresetParameterGroups(UpdateDocumentContributor* parent, const Preset& other);
  PresetParameterGroups(UpdateDocumentContributor* parent, const EditBuffer& eb);

  void copyFromParameterGroupSet(UNDO::Transaction* transaction, const ParameterGroupSet* other);

  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void init(const Preset* preset);
  void init(const ParameterGroupSet* other);

 protected:
  using GroupPtr = std::unique_ptr<PresetParameterGroup>;
  std::unordered_map<std::string, GroupPtr> m_parameterGroups;
};
