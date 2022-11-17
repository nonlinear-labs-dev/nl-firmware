#pragma once

#include <nltools/Types.h>
#include <ParameterId.h>
#include "playground.h"
#include <optional>
#include <parameter_declarations.h>

class Parameter;
class EditBuffer;

class ParameterDB : public sigc::trackable
{
 public:
  explicit ParameterDB(EditBuffer& eb);
  virtual ~ParameterDB();

  [[nodiscard]] Glib::ustring getLongName(const ParameterId& id) const;
  [[nodiscard]] Glib::ustring getShortName(const ParameterId& id) const;
  [[nodiscard]] Glib::ustring getDescription(const ParameterId& id) const;
  [[nodiscard]] std::optional<Glib::ustring> getLongGroupName(const ParameterId& id) const;
  [[nodiscard]] Glib::ustring getDescription(int parameterNumber) const;
  [[nodiscard]] tControlPositionValue getSignalPathIndication(int id) const;
  [[nodiscard]] static C15::Properties::DisplayScalingType getValueDisplayScalingType(const ParameterId& id);
  [[nodiscard]] static C15::Properties::DisplayScalingType getModulationAmountDisplayScalingType(const ParameterId& id);

  static constexpr tControlPositionValue getInvalidSignalPathIndication()
  {
    return std::numeric_limits<tControlPositionValue>::max();
  }

  static tControlPositionValue getCourseDenominator(const ParameterId& id);
  static tControlPositionValue getFineDenominator(const ParameterId& id);
  static tControlPositionValue getCourseModulationDenominator(const ParameterId& id);
  static tControlPositionValue getFineModulationDenominator(const ParameterId& id);
  static double getDefaultValue(const ParameterId& id);

 private:
  [[nodiscard]] Glib::ustring replaceInDynamicLabels(const Glib::ustring& name, const ParameterId& parameterID, SoundType type) const;

  EditBuffer& m_editBuffer;
};
