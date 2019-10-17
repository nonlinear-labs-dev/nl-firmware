#pragma once

#include "AttributesOwner.h"

#include <memory>
#include <unordered_map>

class Preset;
class ParameterGroup;

class PresetParameter;
class PresetParameterGroupSerializer;

using ParameterPtr = std::unique_ptr<PresetParameter>;

class PresetParameterGroup
{
 public:
  PresetParameterGroup();
  PresetParameterGroup(const ::ParameterGroup &other);
  PresetParameterGroup(const PresetParameterGroup &other);
  ~PresetParameterGroup();

  // accessors
  PresetParameter *findParameterByID(int id) const;

  void writeDocument(Writer &writer) const;

  // transactions
  void copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other);
  void copyFrom(UNDO::Transaction *transaction, const ::ParameterGroup *other);

  const std::unordered_map<int, ParameterPtr> &getParameters() const
  {
    return m_parameters;
  }

  // algorithm
  void writeDiff(Writer &writer, const std::string &groupId, const PresetParameterGroup *other) const;

 protected:
  std::unordered_map<int, ParameterPtr> m_parameters;

  friend class PresetParameterGroupSerializer;
};
