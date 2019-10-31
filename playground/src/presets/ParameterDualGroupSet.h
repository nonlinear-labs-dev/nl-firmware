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
  virtual tParameterGroupPtr getParameterGroupByID(const Glib::ustring &id, VoiceGroup vg = VoiceGroup::Invalid) const;
  virtual tParameterGroupPtr getGlobalParameterGroupByID(const Glib::ustring &id) const;
  virtual const IntrusiveList<tParameterGroupPtr> &getParameterGroups(VoiceGroup vg = VoiceGroup::Invalid) const;
  virtual const IntrusiveList<tParameterGroupPtr> &getGlobalParameterGroups() const;

  virtual std::map<int, Parameter *> getParametersSortedById(VoiceGroup vg = VoiceGroup::Invalid) const;
  virtual std::map<int, Parameter *> getGlobalParametersSortedById() const;
  virtual Parameter *findParameterByID(int id, VoiceGroup vgI = VoiceGroup::Invalid) const;
  virtual Parameter *findGlobalParameterByID(int id) const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
  void loadIntoVoiceGroup(UNDO::Transaction *transaction, Preset *p, VoiceGroup target);

 protected:
  virtual ParameterDualGroupSet::tParameterGroupPtr appendParameterGroup(ParameterGroup *p, VoiceGroup v);
  virtual ParameterDualGroupSet::tParameterGroupPtr appendGlobalParameterGroup(ParameterGroup *p);
  virtual void copyFrom(UNDO::Transaction *transaction, const Preset *other);

  void copyVoiceGroup(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to);

 private:
  using tParamGroups = IntrusiveList<tParameterGroupPtr>;
  using tParamMap = std::map<int, Parameter *>;

  //Poly Parameter Groups
  std::array<tParamGroups, 2> m_polyParameterGroups;
  std::array<tParamMap, 2> m_idToParameterMap;

  //Global Parameter Groups (extra master + splitpoint)
  tParamMap m_globalIDToParameterMap;
  tParamGroups m_globalParameterGroups;

  friend class VoiceGroupSerializer;
};
