#include "TimeTools.h"

TimeTools& TimeTools::get()
{
  static TimeTools timeKeeper;
  return timeKeeper;
}

TimeTools::TimeTools()
{
}

TimeTools::~TimeTools()
{

}

time_t TimeTools::getRealTimestamp() const
{
  return time(nullptr);
}

time_t TimeTools::getAdjustedTimestamp() const
{
  auto dateTimeSetting = static_pointer_cast<DateTimeAdjustment>(Application::get().getSettings()->getSetting("DateTimeAdjustment"));

  return getRealTimestamp() + dateTimeSetting->get();
}

Glib::ustring TimeTools::getRealIso()
{
  return getIsoTime(reinterpret_cast<const time_t*>(getRealTimestamp()));
}

Glib::ustring TimeTools::getAdjustedIso()
{
  return getIsoTime(reinterpret_cast<const time_t*>(getAdjustedTimestamp()));
}

Glib::ustring TimeTools::getIsoTime(const time_t* stamp)
{
  char buf[128];
  strftime(buf, sizeof buf, "%FT%TZ", gmtime(stamp));
  return buf;
}

