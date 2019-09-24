#pragma once

#include <http/UpdateDocumentContributor.h>
#include "AttributesOwner.h"
#include "PresetParameterGroup.h"

class Preset;
class ParameterDualGroupSet;
class EditBuffer;

class PresetParameterGroups : public AttributesOwner
{
 public:
  PresetParameterGroups(UpdateDocumentContributor* parent);
  PresetParameterGroups(UpdateDocumentContributor* parent, const Preset& other);
  PresetParameterGroups(UpdateDocumentContributor* parent, const EditBuffer& eb);

  void copyFromParameterGroupSet(UNDO::Transaction* transaction, const ParameterDualGroupSet* other);

  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void init(const Preset* preset);
  void init(const ParameterDualGroupSet* other);

 protected:
  using GroupPtr = std::unique_ptr<PresetParameterGroup>;
  using GroupsMap = std::unordered_map<std::string, GroupPtr>;
  GroupsMap m_parameterGroups;
};
