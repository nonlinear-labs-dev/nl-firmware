#include "EnvelopeReleaseTimeMSScaleConverter.h"
#include "dimension/TimeDimension.h"

EnvelopeReleaseTimeMSScaleConverter::EnvelopeReleaseTimeMSScaleConverter()
    : ScaleConverter(TimeDimension<3>::get())
{
}

EnvelopeReleaseTimeMSScaleConverter::~EnvelopeReleaseTimeMSScaleConverter()
{
}

tDisplayValue EnvelopeReleaseTimeMSScaleConverter::controlPositionToDisplay(const tControlPositionValue &cpValue) const
{
  if(cpValue < (1 / 101.0))
    return 11.3 * cpValue;
  else if(cpValue > 100 / 101.0)
    return std::numeric_limits<tDisplayValue>::infinity();

  return 0.1 * exp((log(16000) - log(0.1)) * cpValue * 1.01);
}

Glib::ustring EnvelopeReleaseTimeMSScaleConverter::controlPositionToDisplayJS() const
{
  std::stringstream s;
  s << "var e = 0;";
  s << "if(cpValue < 1 / 101.0) e = 11.3 * cpValue;";
  s << "else if(cpValue > 100 / 101.0) e = Number.POSITIVE_INFINITY;";
  s << "else e = 0.1 * Math.exp((Math.log(16000) - Math.log(0.1)) * cpValue * 1.01);";
  s << "return $wnd.formatDimension(e, \"" << getDimension().getStingizerJS() << "\", withUnit);";
  return s.str();
}

bool EnvelopeReleaseTimeMSScaleConverter::isBiPolar() const
{
  return false;
}
