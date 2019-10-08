#pragma once
#include "EditorItem.h"

class Parameter;

class ParameterItem : public EditorItem {
public:
  ParameterItem(const Parameter *parameter, const Rect &r);
  MenuOverlay *createOverlay() override;

private:
  const Parameter* m_parameter;
};