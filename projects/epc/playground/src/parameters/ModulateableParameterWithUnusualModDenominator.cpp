#include "ModulateableParameterWithUnusualModDenominator.h"

ModulateableParameterWithUnusualModDenominator::ModulateableParameterWithUnusualModDenominator(
    ParameterGroup *group, ParameterId id, const ScaleConverter *scaling, tDisplayValue def,
    tControlPositionValue coarseDenominator, tControlPositionValue fineDenominator,
    tControlPositionValue modAmountCoarseDenom, tControlPositionValue modAmountFineDenom)
    : ModulateableParameter(group, id, scaling)
    , m_fineModAmountDenominator(modAmountFineDenom)
    , m_coarseModAmountDenominator(modAmountCoarseDenom)
{
}

double ModulateableParameterWithUnusualModDenominator::getModulationAmountFineDenominator() const
{
  return m_fineModAmountDenominator;
}

double ModulateableParameterWithUnusualModDenominator::getModulationAmountCoarseDenominator() const
{
  return m_coarseModAmountDenominator;
}
