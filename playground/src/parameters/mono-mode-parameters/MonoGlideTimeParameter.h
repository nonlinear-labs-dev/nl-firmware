#pragma once
#include "MonoParameter.h"

class MonoGlideTimeParameter : public MonoParameter
{
 public:
  MonoGlideTimeParameter(ParameterGroup* group, ParameterId id);

  Glib::ustring getLongName() const override;

  Glib::ustring getShortName() const override;
};
