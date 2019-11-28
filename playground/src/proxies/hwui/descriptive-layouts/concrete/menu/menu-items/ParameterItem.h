#pragma once
#include "EditorItem.h"

class Parameter;

class ParameterItem : public EditorItem
{
 public:
  ParameterItem(Parameter *const parameter, const Rect &r);
  MenuOverlay *createOverlay() override;

 private:
  Parameter *const m_parameter;
};