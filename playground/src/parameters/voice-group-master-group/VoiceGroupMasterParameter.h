#pragma once
#include <parameters/Parameter.h>
#include <parameters/names/ParameterDB.h>

class VoiceGroupMasterParameter : public Parameter
{
 public:
  VoiceGroupMasterParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling, tControlPositionValue def,
                            tControlPositionValue coarseDenominator, tControlPositionValue fineDenominator,
                            const std::string &shortName, const std::string &longName, VoiceGroup vg);

  ustring getLongName() const override;

  ustring getGroupAndParameterName() const override;

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;

  ustring getShortName() const override;

 private:
  std::string m_shortname;
  std::string m_longname;
  VoiceGroup m_vg;
};
