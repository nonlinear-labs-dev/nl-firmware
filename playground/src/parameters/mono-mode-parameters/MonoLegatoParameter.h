#pragma once
#include "UnmodulateableMonoParameter.h"

class MonoLegatoParameter : public UnmodulateableMonoParameter
{
 public:
  MonoLegatoParameter(ParameterGroup *group, const ParameterId& id);

  Glib::ustring getDisplayString() const override;
  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
};
