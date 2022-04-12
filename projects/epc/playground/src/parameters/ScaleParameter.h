#pragma once

#include <parameters/ModulateableParameter.h>
#include "ModulateableParameterWithUnusualModUnit.h"

namespace ScaleParameterIMPL
{
  Layout* createLayout(FocusAndMode fam) ;
};

class ScaleParameter : public ModulateableParameterWithUnusualModUnit
{
  typedef ModulateableParameterWithUnusualModUnit super;

 public:
  ScaleParameter(ParameterGroup *group, const ParameterId& id, const ScaleConverter *scaling, tControlPositionValue def,
                 tControlPositionValue coarseDenominator, tControlPositionValue fineDenominator);

  Layout *createLayout(FocusAndMode focusAndMode) const override;
  Glib::ustring getMiniParameterEditorName() const override;
  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;
  Glib::ustring getLongName() const override;
  size_t getHash() const override;
};

class BaseScaleParameter : public Parameter
{
  typedef Parameter super;

 public:
  BaseScaleParameter(ParameterGroup* group, const ParameterId& id, const ScaleConverter *scaling, tControlPositionValue def, tControlPositionValue coarseDenom, tControlPositionValue fineDenom);

  Layout* createLayout(FocusAndMode fam) const override;
  Glib::ustring getMiniParameterEditorName() const override;
  void writeDocProperties(Writer& writer, tUpdateID knownRevision) const override;
};
