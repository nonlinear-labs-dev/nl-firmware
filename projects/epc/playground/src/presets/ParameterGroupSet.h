#pragma once

#include "playground.h"
#include <presets/AttributesOwner.h>
#include "tools/IntrusiveList.h"
#include "GroupId.h"
#include "ParameterId.h"
#include "groups/ParameterGroup.h"
#include <vector>
#include <array>
#include <parameters/ParameterTypes.h>

class Parameter;
class Preset;
class ParameterGroup;
class Settings;

class ParameterGroupSet : public AttributesOwner
{
 private:
  using super = AttributesOwner;

 public:
  explicit ParameterGroupSet(UpdateDocumentContributor *parent);
  ~ParameterGroupSet() override;

  virtual void init(OptRefSettings settings);
  [[nodiscard]] bool isInitialized() const;
  typedef ParameterGroup *tParameterGroupPtr;
  typedef IntrusiveList<tParameterGroupPtr> tParamGroups;
  typedef std::map<int, Parameter *> tParamMap;
  typedef std::array<tParamGroups, static_cast<size_t>(VoiceGroup::NumGroups)> tParamArray;
  typedef std::array<tParamMap, static_cast<size_t>(VoiceGroup::NumGroups)> tParamMapArray;

  [[nodiscard]] const tParamArray& getParameters() const;
  [[nodiscard]] virtual tParameterGroupPtr getParameterGroupByID(const GroupId &id) const;
  [[nodiscard]] virtual const IntrusiveList<tParameterGroupPtr> &getParameterGroups(VoiceGroup vg) const;

  [[nodiscard]] virtual std::map<int, Parameter *> getParametersSortedByNumber(VoiceGroup vg) const;

  template <typename tP> tP *findAndCastParameterByID(const ParameterId &id) const
  {
    return dynamic_cast<tP *>(findParameterByID(id));
  }

  [[nodiscard]] virtual Parameter *findParameterByID(const ParameterId &id) const;

  template <VoiceGroup VG> void forEachParameter(const std::function<void(Parameter *)> &cb)
  {
    for(auto g : getParameterGroups(VG))
      for(auto p : g->getParameters())
        cb(p);
  }

  void forEachParameter(VoiceGroup vg, const std::function<void(Parameter *)> &cb);
  void forEachParameter(const std::function<void(Parameter *)> &cb);
  void forEachParameter(const std::function<void(const Parameter *)> &cb) const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

 protected:
  void loadSinglePresetIntoVoiceGroup(UNDO::Transaction *transaction, const Preset *p, VoiceGroup target);
  virtual ParameterGroupSet::tParameterGroupPtr appendParameterGroup(ParameterGroup *p);
  virtual void copyFrom(UNDO::Transaction *transaction, const Preset *other);
  virtual void copyFrom(UNDO::Transaction *transaction, const Preset *other, VoiceGroup from, VoiceGroup to);

  void copyVoiceGroup(UNDO::Transaction *transaction, VoiceGroup from, VoiceGroup to);

 private:
  tParamArray m_parameterGroups;
  tParamMapArray m_idToParameterMap;
  bool m_isInitialized = false;

  void copyGlobalSplitIntoDualSplit(UNDO::Transaction *transaction, const Preset *other);
};
