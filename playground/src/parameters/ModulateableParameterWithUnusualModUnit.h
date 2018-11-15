#pragma once

#include "ModulateableParameter.h"

class ModulateableParameterWithUnusualModUnit : public ModulateableParameter
{
 private:
  typedef ModulateableParameter super;

 public:
  ModulateableParameterWithUnusualModUnit(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                                          const ScaleConverter *modAmountScaling, tDisplayValue def,
                                          int coarseDenominator, int fineDenominator);

  void undoableIncrementMCAmount(UNDO::Scope::tTransactionPtr transaction, int inc, ButtonModifiers modifiers) override;
  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;
  Glib::ustring stringizeModulationAmount() const override;
  double getModulationAmountFineDenominator() const override;
  double getModulationAmountCoarseDenominator() const override;

 private:
  const ScaleConverter *m_modAmountScaling;
};
