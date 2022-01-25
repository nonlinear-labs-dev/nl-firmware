#pragma once

#include <parameters/ModulateableParameter.h>

class ScaleParameter : public ModulateableParameter
{
  typedef ModulateableParameter super;

 public:
  ScaleParameter(ParameterGroup *group, const ParameterId& id, const ScaleConverter *scaling, tControlPositionValue def,
                 tControlPositionValue coarseDenominator, tControlPositionValue fineDenominator);

  Layout *createLayout(FocusAndMode focusAndMode) const override;
  Glib::ustring getMiniParameterEditorName() const override;
  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;
  Glib::ustring getLongName() const override;
  size_t getHash() const override;
};
