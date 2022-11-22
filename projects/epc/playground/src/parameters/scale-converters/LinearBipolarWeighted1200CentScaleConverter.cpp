#include "LinearBipolarWeighted1200CentScaleConverter.h"
#include "dimension/CentDimension.h"
#include "nltools/Assert.h"

LinearBipolarWeighted1200CentScaleConverter::LinearBipolarWeighted1200CentScaleConverter()
    : ScaleConverter(CentDimension::get())
    , m_displayRange(tDisplayRange(-1200, 1200))
{
}
LinearBipolarWeighted1200CentScaleConverter::~LinearBipolarWeighted1200CentScaleConverter()
{
}

tDisplayValue
    LinearBipolarWeighted1200CentScaleConverter::controlPositionToDisplay(const tControlPositionValue& cpValue) const
{
  const auto Lim = 10. / 21.;  // 0.47619

  if((cpValue > -Lim) && (cpValue < Lim))
  {
    return 100 * cpValue / Lim;
  }
  else if(cpValue >= Lim)
  {
    return 100 + 1100 * (cpValue - Lim) / (1.0 - Lim);
  }
  else
  {
    return -100 + 1100 * (cpValue + Lim) / (1.0 - Lim);
  }
}

Glib::ustring LinearBipolarWeighted1200CentScaleConverter::controlPositionToDisplayJS() const
{
  std::stringstream s;
  s << "var e = 0;";
  s << "var lim = 10./21.;";
  s << "if((cpValue > -lim) && (cpValue < lim)) e = 100 * cpValue / lim;";
  s << "else if(cpValue >= lim) e = 100 + 1100 * (cpValue - lim) / (1.0 - lim);";
  s << "else if(cpValue <= -lim) e = -100 + 1100 * (cpValue + lim) / (1.0 - lim);";
  s << "return $wnd.formatDimension(e, \"" << getDimension().getStingizerJS() << "\", withUnit);";
  return s.str();
}

bool LinearBipolarWeighted1200CentScaleConverter::isBiPolar() const
{
  return m_displayRange.isBiPolar();
}
