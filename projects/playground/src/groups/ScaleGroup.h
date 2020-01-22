#pragma once

#include <groups/ParameterGroup.h>
#include <nltools/threading/Throttler.h>

class ParameterDualGroupSet;

class ScaleGroup : public ParameterGroup
{
 public:
  ScaleGroup(ParameterDualGroupSet *parent);
  virtual ~ScaleGroup();

  void init();
  bool isAnyOffsetChanged() const;
  static int getScaleBaseParameterNumber();

  static bool isScaleParameter(const ParameterId& id);
  static bool isScaleParameter(const Parameter* parameter);

private:
  void onBaseKeyParameterChanged(const Parameter *baseKeyParameter);

  Throttler m_updateNames;
};
