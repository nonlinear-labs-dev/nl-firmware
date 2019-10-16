#pragma once
#include "ModuleCaption.h"

class DualSpecialParameterModuleCaption : public ModuleCaption
{
public:
  DualSpecialParameterModuleCaption(const Rect& pos);
  void updateText(Parameter *newOne) override;
};