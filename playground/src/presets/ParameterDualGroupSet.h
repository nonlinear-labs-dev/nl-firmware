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

  virtual tParameterGroupPtr getParameterGroupByID(const Glib::ustring &id, VoiceGroup vg) const;
  virtual const IntrusiveList<tParameterGroupPtr> &getParameterGroups(VoiceGroup vg) const;

  virtual std::map<int, Parameter *> getParametersSortedById(VoiceGroup vg) const;
  virtual Parameter *findParameterByID(int id, VoiceGroup vg) const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

 protected:
  void loadIntoVoiceGroup(UNDO::Transaction *transaction, Preset *p, VoiceGroup target);
  virtual ParameterDualGroupSet::tParameterGroupPtr appendParameterGroup(ParameterGroup *p, VoiceGroup v);
  virtual void copyFrom(UNDO::Transaction *transaction, const Preset *other);

  void copyVoiceGroup(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to);

 private:
  using tParamGroups = IntrusiveList<tParameterGroupPtr>;
  using tParamMap = std::map<int, Parameter *>;

  std::array<tParamGroups, static_cast<size_t>(VoiceGroup::NumGroups)> m_parameterGroups;
  std::array<tParamMap, static_cast<size_t>(VoiceGroup::NumGroups)> m_idToParameterMap;

  friend class VoiceGroupSerializer;
};
