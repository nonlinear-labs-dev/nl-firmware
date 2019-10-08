#include "MonoParameter.h"

MonoParameter::MonoParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                             tControlPositionValue def, tControlPositionValue coarseDenominator,
                             tControlPositionValue fineDenominator)
    : Parameter(group, id, scaling, def, coarseDenominator, fineDenominator)
{
}
