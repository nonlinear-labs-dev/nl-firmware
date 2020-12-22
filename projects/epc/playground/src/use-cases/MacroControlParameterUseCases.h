#pragma once

#include <ParameterId.h>
#include <parameters/MacroControlParameter.h>
class EditBuffer;

class MacroControlParameterUseCases
{
 public:
  explicit MacroControlParameterUseCases(MacroControlParameter* mc);

  void setName(const Glib::ustring& name);
  void setInfo(const Glib::ustring& name);
  void resetConnectionsToTargets();

  void incDecPosition(int incs, bool fine, bool shift);

 private:
  MacroControlParameter* m_mc;
};
