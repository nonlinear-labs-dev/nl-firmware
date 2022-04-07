#pragma once

#include <parameters/ModulateableParameter.h>

class ScaleParameterIMPL
{
 public:
  Layout* createLayout(FocusAndMode fam) const;

};

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

 private:
  ScaleParameterIMPL m_scaleImpl;
};

class BaseScaleParameter : public Parameter
{
  typedef Parameter super;

 public:
  BaseScaleParameter(ParameterGroup* group, const ParameterId& id, const ScaleConverter *scaling, tControlPositionValue def, tControlPositionValue coarseDenom, tControlPositionValue fineDenom);

  Layout* createLayout(FocusAndMode fam) const override;
  Glib::ustring getMiniParameterEditorName() const override;
  void writeDocProperties(Writer& writer, tUpdateID knownRevision) const override;

 private:
  ScaleParameterIMPL m_scaleImpl;
};
