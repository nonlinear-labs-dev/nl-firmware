#pragma once

#include <groups/ParameterGroup.h>
#include <nltools/threading/Throttler.h>

class ParameterDualGroupSet;

class ScaleGroup : public ParameterGroup
{
 public:
  ScaleGroup(ParameterDualGroupSet *parent, VoiceGroup voicegroup);
  virtual ~ScaleGroup();

  void init();
  bool isAnyOffsetChanged() const;
  static int getScaleBaseParameterID();

 private:
  void onBaseKeyParameterChanged(const Parameter *baseKeyParameter);

  Throttler m_updateNames;
};
