#pragma once

#include "AttributesOwner.h"

#include <memory>
#include <unordered_map>

class Preset;
class ParameterGroup;

class PresetParameter;
class PresetParameterGroupSerializer;

using ParameterPtr = std::unique_ptr<PresetParameter>;

class PresetParameterGroup : public UpdateDocumentContributor
{
 public:
  PresetParameterGroup(Preset *parent);
  PresetParameterGroup(Preset *parent, const ::ParameterGroup &other);
  PresetParameterGroup(Preset *parent, const PresetParameterGroup &other);
  ~PresetParameterGroup() override;

  // supported interfaces
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  // accessors
  PresetParameter *findParameterByID(int id) const;

  // transactions
  void copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other);
  void copyFrom(UNDO::Transaction *transaction, const ::ParameterGroup *other);

  // algorithm
  void writeDiff(Writer &writer, const std::string &groupId, const PresetParameterGroup *other) const;

 private:
  std::unordered_map<int, ParameterPtr> m_parameters;

  friend class PresetParameterGroupSerializer;
};
