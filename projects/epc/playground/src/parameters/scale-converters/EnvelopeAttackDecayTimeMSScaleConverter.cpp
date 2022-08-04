#include "EnvelopeAttackDecayTimeMSScaleConverter.h"
#include "dimension/TimeDimension.h"

EnvelopeAttackDecayTimeMSScaleConverter::EnvelopeAttackDecayTimeMSScaleConverter()
    : ScaleConverter(TimeDimension<3>::get())
{
}

EnvelopeAttackDecayTimeMSScaleConverter::~EnvelopeAttackDecayTimeMSScaleConverter()
{
}

tDisplayValue
    EnvelopeAttackDecayTimeMSScaleConverter::controlPositionToDisplay(const tControlPositionValue &cpValue) const
{
  if(cpValue < 0.01)
    return 11.3 * cpValue;

  return 0.1 * exp((log(16000) - log(0.1)) * cpValue);
}

Glib::ustring EnvelopeAttackDecayTimeMSScaleConverter::controlPositionToDisplayJS() const
{
  std::stringstream s;
  s << "var e = 0;";
  s << "if(cpValue < 0.01) e = 11.3 * cpValue;";
  s << "else e = 0.1 * Math.exp((Math.log(16000) - Math.log(0.1)) * cpValue);";
  s << "return $wnd.formatDimension(e, \"" << getDimension().getStingizerJS() << "\", withUnit);";
  return s.str();
}

bool EnvelopeAttackDecayTimeMSScaleConverter::isBiPolar() const
{
  return false;
}