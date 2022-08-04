#include "EnvelopeDecayTimeMSScaleConverter.h"
#include "dimension/TimeDimension.h"

EnvelopeDecayTimeMSScaleConverter::EnvelopeDecayTimeMSScaleConverter()
    : ScaleConverter(TimeDimension<3>::get())
    , m_displayRange(0.0, 16000.0)
{
}

EnvelopeDecayTimeMSScaleConverter::~EnvelopeDecayTimeMSScaleConverter()
{
}

tDisplayValue EnvelopeDecayTimeMSScaleConverter::controlPositionToDisplay(const tControlPositionValue &cpValue) const
{
  if(cpValue < 0.01)
    return 11.3 * cpValue;

  return 0.1 * exp(log(16000) - log(0.1) * cpValue);
}

Glib::ustring EnvelopeDecayTimeMSScaleConverter::controlPositionToDisplayJS() const
{
  std::stringstream s;
  s << "var e = 0;";
  s << "if(cpValue < 0.01) e = 11.3 * cpValue;";
  s << "else e = 0.1 * Math.exp(Math.log(16000) - Math.log(0.1) * cpValue);";
  s << "return $wnd.formatDimension(e, \"" << getDimension().getStingizerJS() << "\", withUnit);";
  return s.str();
}

bool EnvelopeDecayTimeMSScaleConverter::isBiPolar() const
{
  return false;
}
