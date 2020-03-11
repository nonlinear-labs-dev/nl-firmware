#pragma once

#include <parameters/Parameter.h>
#include <parameters/names/ParameterDB.h>
#include <parameters/ModulateableParameter.h>

class VoiceGroupMasterModulateableParameter : public ModulateableParameter
{
 public:
  VoiceGroupMasterModulateableParameter(ParameterGroup *group, const ParameterId &id, const ScaleConverter *scaling,
                                        tControlPositionValue def, tControlPositionValue coarseDenominator,
                                        tControlPositionValue fineDenominator);

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;
};

class VoiceGroupMasterUnmodulateableParameter : public Parameter
{
 public:
  using Parameter::Parameter;

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;
};