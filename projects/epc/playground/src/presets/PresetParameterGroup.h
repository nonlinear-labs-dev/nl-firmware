#pragma once

#include "AttributesOwner.h"
#include "ParameterId.h"
#include <memory>
#include <vector>

class Preset;
class ParameterGroup;

class PresetParameter;
class PresetParameterGroupSerializer;

class GroupId;

using ParameterPtr = std::unique_ptr<PresetParameter>;

class PresetParameterGroup
{
 public:
  explicit PresetParameterGroup(const ::ParameterGroup &other);
  PresetParameterGroup(const PresetParameterGroup &other);
  ~PresetParameterGroup();

  // accessors
  [[nodiscard]] PresetParameter *findParameterByID(ParameterId id) const;
  [[nodiscard]] PresetParameter *findParameterByNumber(uint16_t num) const;

  void writeDocument(Writer &writer) const;

  // transactions
  void copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other);
  void copyFrom(UNDO::Transaction *transaction, const ::ParameterGroup *other);
  void assignVoiceGroup(UNDO::Transaction *transaction, VoiceGroup vg);

  [[nodiscard]] const std::vector<ParameterPtr> &getParameters() const
  {
    return m_parameters;
  }

  [[nodiscard]] VoiceGroup getVoiceGroup() const;

  // algorithm
  void writeDiff(Writer &writer, const GroupId &groupId, const PresetParameterGroup *other) const;

 private:
  std::vector<ParameterPtr> m_parameters;
  VoiceGroup m_voiceGroup;

  friend class PresetParameterGroupSerializer;
};
