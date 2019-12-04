#pragma once

#include "MonoParameter.h"

class MonoLegatoParameter : public MonoParameter
{
 public:
  MonoLegatoParameter(ParameterGroup *group, ParameterId id);
  Glib::ustring getDisplayString() const override;

  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
};
