#pragma once

#include "playground.h"
#include "groups/ParameterGroup.h"
#include <presets/AttributesOwner.h>
#include <vector>
#include "tools/IntrusiveList.h"

class Parameter;
class Preset;

class ParameterGroupSet : public AttributesOwner
{
 private:
  using super = AttributesOwner;

 public:
  ParameterGroupSet(UpdateDocumentContributor *parent);
  ~ParameterGroupSet() override;

  typedef ParameterGroup *tParameterGroupPtr;

  size_t countParameters() const;
  tParameterGroupPtr getParameterGroupByID(const Glib::ustring &id) const;

  const IntrusiveList<tParameterGroupPtr> &getParameterGroups() const
  {
    return m_parameterGroups;
  }

  std::map<int, Parameter *> getParametersSortedById() const;
  Parameter *findParameterByID(int id) const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

 protected:
  void copyFrom(UNDO::Transaction *transaction, const Preset *other);
  tParameterGroupPtr appendParameterGroup(ParameterGroup *p);

 private:
  IntrusiveList<tParameterGroupPtr> m_parameterGroups;
  std::map<int, Parameter *> m_idToParameterMap;
};
