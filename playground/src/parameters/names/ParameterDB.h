#pragma once

#include "playground.h"

class Parameter;

class ParameterDB
{
 public:
  static ParameterDB &get();

  virtual ~ParameterDB();

  Glib::ustring getLongName(int id) const;
  Glib::ustring getShortName(int id) const;
  tControlPositionValue getSignalPathIndication(int id) const;

  static constexpr tControlPositionValue getInvalidSignalPathIndication()
  {
    return std::numeric_limits<tControlPositionValue>::max();
  }

  bool isActive(const Parameter *p) const;

 private:

  Glib::ustring replaceVoiceGroupInDynamicLabels(Glib::ustring name) const;
  ParameterDB();
};
