#pragma once

#include <glibmm/ustring.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/DateTimeAdjustment.h>
#include <chrono>

class TimeTools
{
  public:
    static TimeTools& get();

    Glib::ustring getRealIso();
    Glib::ustring getAdjustedIso();

    time_t getRealTimestamp() const;
    time_t getAdjustedTimestamp() const;

  private:
    TimeTools();
    virtual ~TimeTools();
    Glib::ustring getIsoTime(const time_t* stamp);
};
