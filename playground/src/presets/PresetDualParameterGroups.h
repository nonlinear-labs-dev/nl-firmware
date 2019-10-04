#pragma once

#include <http/UpdateDocumentContributor.h>
#include <presets/dual/SplitPresetParameterGroup.h>
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

protected:
  using GroupPtr = std::unique_ptr<PresetParameterGroup>;
  using GroupsMap = std::unordered_map<std::string, GroupPtr>;
  SoundType m_type;
  std::array<GroupsMap, 2> m_parameterGroups;
  SplitPresetParameterGroup m_splitGroup;

  friend class PresetParameterVoiceGroupSerializer;
  friend class PresetParameterGroupsSerializer;
};
