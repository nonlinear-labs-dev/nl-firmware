#pragma once

#include "playground.h"
#include <http/UpdateDocumentContributor.h>
#include <unordered_map>
#include <type_traits>

namespace UNDO
{
  class Transaction;
}

class Parameter;

class PresetParameterGroup;

class PresetParameter : public UpdateDocumentContributor
{
 public:
  PresetParameter(PresetParameterGroup *parent, int id);
  PresetParameter(PresetParameterGroup *parent, const ::Parameter &other);
  PresetParameter(PresetParameterGroup *parent, const PresetParameter &other);

  // supported interfaces
  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

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
  ModulationSource getModulationSource() const;
  double getModulationAmount() const;
  std::string getGivenName() const;
  std::string getInfo() const;
  enum RibbonReturnMode getRibbonReturnMode() const;
  enum RibbonTouchBehaviour getRibbonTouchBehaviour() const;
  enum PedalModes getPedalMode() const;

  // transactions
  void setValue(UNDO::Transaction *transaction, tControlPositionValue v);
  void setField(UNDO::Transaction *transaction, Fields field, const std::string &value);
  void copyFrom(UNDO::Transaction *transaction, const PresetParameter *other);
  void copyFrom(UNDO::Transaction *transaction, const ::Parameter *other);

  // algorithm
  void writeDiff(Writer &writer, int parameterID, const PresetParameter *other) const;

 private:
  int m_id;
  tControlPositionValue m_value = 0;
  std::unordered_map<Fields, std::string, std::hash<uint8_t>> m_fields;

  friend class PresetParameterSerializer;
};

std::string paramFieldToString(PresetParameter::Fields f);
