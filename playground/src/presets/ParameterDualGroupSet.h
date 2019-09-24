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
  tParameterGroupPtr getParameterGroupByID(const Glib::ustring &id) const;
  tParameterGroupPtr getParameterGroupByID(const Glib::ustring &id, VoiceGroup vg) const;

  const IntrusiveList<tParameterGroupPtr> &getParameterGroups(VoiceGroup vg) const;
  const IntrusiveList<tParameterGroupPtr> &getParameterGroups() const;

  std::map<int, Parameter *> getParametersSortedById() const;
  std::map<int, Parameter *> getParametersSortedById(VoiceGroup vg) const;

  Parameter *findParameterByID(int id, VoiceGroup vgI) const;
  Parameter *findParameterByID(int id) const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

 protected:
  void copyFrom(UNDO::Transaction *transaction, const Preset *other, VoiceGroup target);
  void copyFrom(UNDO::Transaction *transaction, const Preset *other);
  ParameterDualGroupSet::tParameterGroupPtr appendParameterGroup(ParameterGroup *p, VoiceGroup v);

  VoiceGroup m_selectedVoiceGroup = VoiceGroup::I;

private:
  using tParamGroups = IntrusiveList<tParameterGroupPtr>;
  using tParamMap = std::map<int, Parameter *>;

  std::array<tParamGroups, 2> m_parameterGroups;
  std::array<tParamMap, 2> m_idToParameterMap;

  friend class VoiceGroupSerializer;

  void selectVoiceGroup(VoiceGroup group);
};
