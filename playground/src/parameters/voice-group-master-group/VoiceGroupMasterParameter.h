#pragma once

#include <parameters/Parameter.h>
#include <parameters/names/ParameterDB.h>
#include <parameters/ModulateableParameter.h>

class VoiceGroupMasterParameter : public ModulateableParameter
{
 public:
  VoiceGroupMasterParameter(ParameterGroup *group, const ParameterId &id, const ScaleConverter *scaling,
                            tControlPositionValue def, tControlPositionValue coarseDenominator,
                            tControlPositionValue fineDenominator, std::string shortName, std::string longName,
                            VoiceGroup vg);

  Glib::ustring getLongName() const override;

  Glib::ustring getGroupAndParameterName() const override;

  DFBLayout *createLayout(FocusAndMode focusAndMode) const override;

  Glib::ustring getShortName() const override;

 private:
  std::string m_shortname;
  std::string m_longname;
  VoiceGroup m_vg;
};
