#pragma once

#include <http/UpdateDocumentContributor.h>
#include "AttributesOwner.h"
#include "PresetParameterGroup.h"

class Preset;
class ParameterDualGroupSet;
class EditBuffer;

class PresetDualParameterGroups : public AttributesOwner
{
 public:
  PresetDualParameterGroups(UpdateDocumentContributor* parent);
  PresetDualParameterGroups(UpdateDocumentContributor* parent, const Preset& other);
  PresetDualParameterGroups(UpdateDocumentContributor* parent, const EditBuffer& eb);

  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void init(const Preset* preset);
  void init(const ParameterDualGroupSet* other);

 protected:
  using GroupPtr = std::unique_ptr<PresetParameterGroup>;
  using GroupsMap = std::unordered_map<std::string, GroupPtr>;
  std::array<GroupsMap, 2> m_parameterGroups;

  friend class PresetParameterVoiceGroupSerializer;
  friend class PresetParameterGroupsSerializer;
};
