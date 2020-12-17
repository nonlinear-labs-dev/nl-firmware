#pragma once

#include <glibmm/ustring.h>
class PedalParameter;

class PedalParameterUseCases
{
 public:
  explicit PedalParameterUseCases(PedalParameter* p);

  void setPedalMode(const Glib::ustring& mode);

 private:
  PedalParameter* m_param;
};
