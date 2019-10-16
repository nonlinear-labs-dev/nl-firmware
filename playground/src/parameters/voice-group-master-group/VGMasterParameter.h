#pragma once
#include <parameters/Parameter.h>
#include <parameters/names/ParameterDB.h>

class VGMasterParameter : public Parameter
{
 public:
  VGMasterParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling, tControlPositionValue def,
                    tControlPositionValue coarseDenominator, tControlPositionValue fineDenominator);

  ustring getLongName() const override;

  ustring getGroupAndParameterName() const override;

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;

  ustring getShortName() const override;
};

