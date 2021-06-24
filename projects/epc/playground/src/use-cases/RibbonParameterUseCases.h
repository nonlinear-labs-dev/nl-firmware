#pragma once

#include <glibmm/ustring.h>
class RibbonParameter;

class RibbonParameterUseCases
{
 public:
  explicit RibbonParameterUseCases(RibbonParameter* p);
  void setTouchBehaviour(const Glib::ustring& mode);
  void setReturnMode(const Glib::ustring& mode);

  void incTouchBehaviour();

 private:
  RibbonParameter* m_param;
};
