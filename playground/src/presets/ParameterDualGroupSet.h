#pragma once

#include "playground.h"
#include "groups/ParameterGroup.h"
#include <presets/AttributesOwner.h>
#include <vector>
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
  tParameterGroupPtr getParameterGroupByID(const Glib::ustring &id, VoiceGroup vg = VoiceGroup::I) const;

  const IntrusiveList<tParameterGroupPtr> &getParameterGroups(VoiceGroup vg = VoiceGroup::I) const
  {
    return m_parameterGroups[static_cast<int>(vg)];
  }

  std::map<int, Parameter *> getParametersSortedById(VoiceGroup vg = VoiceGroup::I) const;
  Parameter *findParameterByID(int id, VoiceGroup vg = VoiceGroup::I) const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

 protected:
  void copyFrom(UNDO::Transaction *transaction, const Preset *other, VoiceGroup target = VoiceGroup::I);
  ParameterDualGroupSet::tParameterGroupPtr appendParameterGroup(ParameterGroup *p, VoiceGroup v = VoiceGroup::I);

 private:
  using tParamGroups = IntrusiveList<tParameterGroupPtr>;
  using tParamMap = std::map<int, Parameter *>;

  std::array<tParamGroups, 2> m_parameterGroups;
  std::array<tParamMap, 2> m_idToParameterMap;
};
