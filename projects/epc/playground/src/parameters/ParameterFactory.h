#pragma once
#include <vector>
#include <string>
#include "nltools/Types.h"
#include "ParameterId.h"

class Parameter;
class ParameterGroup;

class ParameterFactory
{
 public:
  static std::vector<int> getParameterIDs(const std::string& longName);
  static bool isModulateable(int id);
  //todo change to use ParameterId instead of 2 args
  static Parameter* createParameterByType(ParameterGroup* parent, const ParameterId& id);

  static bool isUnisonParameter(const Parameter* parameter);
  static bool isUnisonParameter(const ParameterId& id);
  static bool isSplitPoint(const Parameter* p);
  static bool isSplitPoint(const ParameterId& id);
  static bool isMonoParameter(const Parameter* parameter);
  static bool isMonoParameter(const ParameterId& id);
  static bool isVoiceGroupMasterParameter(const ParameterId& id);
  static bool isMasterParameter(const Parameter* p);
  static bool isScaleParameter(const ParameterId& id);
  static bool isScaleOffsetParameter(const ParameterId& id);
  static bool isScaleParameter(const Parameter* parameter);
};