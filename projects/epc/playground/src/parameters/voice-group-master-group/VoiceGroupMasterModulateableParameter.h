#pragma once

#include <parameters/Parameter.h>
#include <parameters/names/ParameterDB.h>
#include <parameters/ModulateableParameter.h>

class VoiceGroupMasterModulateableParameter : public ModulateableParameter
{
 public:
  VoiceGroupMasterModulateableParameter(ParameterGroup *group, const ParameterId &id);
  Layout *createLayout(FocusAndMode focusAndMode) const override;
};

class VoiceGroupMasterUnmodulateableParameter : public Parameter
{
 public:
  using Parameter::Parameter;

  Layout *createLayout(FocusAndMode focusAndMode) const override;
};