#pragma once
#include "ArrowIncrementDecrementOverlay.h"

class Parameter;
class ParameterValueLabel;

class ParameterOverlay : public ArrowIncrementDecrementOverlay
{
 public:
  explicit ParameterOverlay(const Rect &rect, const Parameter *parameter);
  void onLeft(bool down) override;
  void onRight(bool down) override;
private:
  Parameter* getNonConstParameter(const Parameter* param) const;

  ParameterValueLabel* m_label;
  const Parameter* m_parameter;
};