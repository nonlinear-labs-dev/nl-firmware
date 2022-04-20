#pragma once

#include "playground.h"
#include "AttributesOwner.h"
#include <vector>
#include <type_traits>
#include <ParameterId.h>

namespace UNDO
{
  class Transaction;
}

class Parameter;
class PresetParameterGroup;
class Writer;

class PresetParameter
{
 public:
  explicit PresetParameter(const ParameterId &id);
  explicit PresetParameter(const ::Parameter &other);
  PresetParameter(const PresetParameter &other);

  enum Fields : uint8_t
  {
    ModAmount,
    ModSource,
    GivenName,
    Info,
    RibbonTouchBehaviour,
    RibbonReturnMode,
    PedalMode
  };

  // accessors
  tControlPositionValue getValue() const;
  MacroControls getModulationSource() const;
  double getModulationAmount() const;
  std::string getGivenName() const;
  std::string getInfo() const;
  const ParameterId getID() const;
  enum RibbonReturnMode getRibbonReturnMode() const;
  enum RibbonTouchBehaviour getRibbonTouchBehaviour() const;
  enum PedalModes getPedalMode() const;
  void assignVoiceGroup(VoiceGroup vg);

  // transactions
  void setValue(UNDO::Transaction *transaction, tControlPositionValue v);
  void setField(UNDO::Transaction *transaction, Fields field, const std::string &value);
  void copyFrom(UNDO::Transaction *transaction, const std::unique_ptr<PresetParameter> &other);
  void copyFrom(UNDO::Transaction *transaction, const PresetParameter *other);
  void copyFrom(UNDO::Transaction *transaction, const ::Parameter *other);

  // algorithm
  void writeDiff(Writer &writer, ParameterId parameterID, const PresetParameter *other) const;
  void writeDocument(Writer &writer) const;

 private:
  using FieldsStorage = std::vector<std::tuple<Fields, std::string>>;
  ParameterId m_id;
  tControlPositionValue m_value = 0;
  FieldsStorage m_fields;

  friend class PresetParameterSerializer;
};

std::string paramFieldToString(PresetParameter::Fields f);
