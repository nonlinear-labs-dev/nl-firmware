#pragma once

#include "playground.h"
#include "groups/ParameterGroup.h"
#include <presets/AttributesOwner.h>
#include <vector>
#include <groups/SplitSoundGroup.h>
#include "tools/IntrusiveList.h"

class Parameter;
class Preset;

class ParameterDualGroupSet : public AttributesOwner
{
 private:
  using super = AttributesOwner;

 public:
  ParameterDualGroupSet(UpdateDocumentContributor *parent);
  ~ParameterDualGroupSet() override;

  typedef ParameterGroup *tParameterGroupPtr;

  size_t countParameters() const;
  virtual tParameterGroupPtr getParameterGroupByID(const Glib::ustring &id, VoiceGroup vg = VoiceGroup::Invalid) const;
  virtual const IntrusiveList<tParameterGroupPtr> &getParameterGroups(VoiceGroup vg = VoiceGroup::Invalid) const;
  virtual std::map<int, Parameter *> getParametersSortedById(VoiceGroup vg = VoiceGroup::Invalid) const;
  virtual Parameter *findParameterByID(int id, VoiceGroup vgI = VoiceGroup::Invalid) const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  virtual SplitSoundGroup *getSplitSoundParameterGroup();
  virtual const SplitSoundGroup *getSplitSoundParameterGroup() const;

 protected:
  virtual ParameterDualGroupSet::tParameterGroupPtr appendParameterGroup(ParameterGroup *p, VoiceGroup v);
  virtual void copyFrom(UNDO::Transaction *transaction, const Preset *other);

  void copyVoiceGroup(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to);

 private:
  using tParamGroups = IntrusiveList<tParameterGroupPtr>;
  using tParamMap = std::map<int, Parameter *>;

  std::array<tParamGroups, 2> m_parameterGroups;
  std::array<tParamMap, 2> m_idToParameterMap;

  friend class VoiceGroupSerializer;

  SplitSoundGroup m_splitParameters;
};
