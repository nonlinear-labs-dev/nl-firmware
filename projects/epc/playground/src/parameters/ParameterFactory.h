#pragma once
#include <vector>
#include <string>
#include "nltools/Types.h"

class Parameter;
class ParameterGroup;

class ParameterFactory
{
 public:
  static std::vector<int> getParameterIDs(const std::string& longName);
  static bool isModulateable(int id);
  static Parameter* createParameterByType(ParameterGroup* parent, int id, VoiceGroup vg);
};