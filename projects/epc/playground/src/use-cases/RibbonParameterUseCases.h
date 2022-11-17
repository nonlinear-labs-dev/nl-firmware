#pragma once

#include <glibmm/ustring.h>
#include <use-cases/PhysicalControlParameterUseCases.h>

class RibbonParameter;

class RibbonParameterUseCases : public PhysicalControlParameterUseCases
{
 public:
  explicit RibbonParameterUseCases(RibbonParameter* p);
  void setTouchBehaviour(const Glib::ustring& mode);
  void setReturnMode(const RibbonReturnMode& mode);
  void setReturnMode(const Glib::ustring& mode);
  void setTouchBehaviour(RibbonTouchBehaviour mode);

  void incTouchBehaviour();

  void stepTouchBehaviour(int i);

 private:
  RibbonParameter* m_param;
};
