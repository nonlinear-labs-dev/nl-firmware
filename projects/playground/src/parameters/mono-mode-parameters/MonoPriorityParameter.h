#pragma once
#include "UnmodulateableMonoParameter.h"

class MonoPriorityParameter : public UnmodulateableMonoParameter
{
 public:
  MonoPriorityParameter(ParameterGroup* parent, const ParameterId& id);

  Glib::ustring getDisplayString() const override;
  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
};
