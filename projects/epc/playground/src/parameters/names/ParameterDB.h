#pragma once

#include <nltools/Types.h>
#include <ParameterId.h>
#include "playground.h"
#include <optional>

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

  static constexpr tControlPositionValue getInvalidSignalPathIndication()
  {
    return std::numeric_limits<tControlPositionValue>::max();
  }

 private:
  [[nodiscard]] Glib::ustring replaceInDynamicLabels(Glib::ustring name, VoiceGroup originGroup, SoundType type) const;

  EditBuffer& m_editBuffer;
};
