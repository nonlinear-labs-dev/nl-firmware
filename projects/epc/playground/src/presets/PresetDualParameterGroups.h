#pragma once

#include <http/UpdateDocumentContributor.h>
#include "AttributesOwner.h"
#include "GroupId.h"
#include "ParameterId.h"
#include "PresetParameterGroup.h"
#include <set>
#include <optional>

class Preset;
class ParameterGroupSet;
class PresetParameterGroup;
class EditBuffer;
class PresetParameter;

class PresetDualParameterGroups : public AttributesOwner
{
 public:
  using GroupPtr = std::unique_ptr<PresetParameterGroup>;
  using GroupsMap = std::map<GroupId, GroupPtr>;
  typedef std::vector<std::pair<GroupId, const PresetParameterGroup *>> tParameterGroups;

  explicit PresetDualParameterGroups(UpdateDocumentContributor* parent);
  PresetDualParameterGroups(UpdateDocumentContributor* parent, const Preset& other);
  PresetDualParameterGroups(UpdateDocumentContributor* parent, const EditBuffer& eb);
  ~PresetDualParameterGroups() override;

  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  void init(const Preset* preset);

  GroupsMap& getGroups(VoiceGroup vg);

  SoundType getType() const;

  virtual void setType(UNDO::Transaction* transaction, SoundType type);

  void forEachParameter(const std::function<void(const PresetParameter*)>& cb) const;
  void forEachParameter(const std::function<void(PresetParameter*)>& cb);

  template <VoiceGroup VG> void forEachParameter(const std::function<void(PresetParameter*)>& cb)
  {
    for(auto& g : m_parameterGroups[static_cast<int>(VG)])
      for(auto& p : g.second->getParameters())
        cb(p.get());
  }

  [[nodiscard]] PresetParameterGroup* findParameterGroup(const GroupId& id) const;
  [[nodiscard]] PresetParameter* findParameterByID(ParameterId id, bool throwIfMissing) const;

  void copyVoiceGroup1IntoVoiceGroup2(UNDO::Transaction* transaction, std::optional<std::set<GroupId> > whiteList);

  void writeGroups(Writer& writer, const Preset* other, VoiceGroup vgOfThis, VoiceGroup vgOfOther) const;
  PresetParameterGroup* findOrCreateParameterGroup(const GroupId& id);

  [[nodiscard]] tParameterGroups getGroups(const VoiceGroup& vg) const;

 protected:
  void copyFrom(UNDO::Transaction* transaction, const PresetDualParameterGroups* other);
  void copyFrom(UNDO::Transaction* transaction, const EditBuffer* other);

 private:
  void initEmpty();

  SoundType m_type;
  std::array<GroupsMap, static_cast<size_t>(VoiceGroup::NumGroups)> m_parameterGroups;

  static ParameterGroupSet& getDataScheme();

  friend class PresetParameterVoiceGroupSerializer;
  friend class PresetParameterGroupsSerializer;
};
