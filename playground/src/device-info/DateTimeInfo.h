#pragma once

#include "DeviceInformationItem.h"

class DateTimeInfo : public DeviceInformationItem
{
  public:
    DateTimeInfo (DeviceInformation *parent);

    virtual void writeDocument (Writer &writer, tUpdateID knownRevision) const override;
    Glib::ustring get() const override;
    Glib::ustring getDisplayString () const override;

    static Glib::ustring formatTime (uint64_t secondsSinceUnixEpoch, const Glib::ustring &format);
    static Glib::ustring formatTime (const Glib::ustring &iso, const Glib::ustring &format);
    static Glib::ustring getIsoStringOfNow();
    static Glib::ustring getDisplayStringFromIso(const Glib::ustring & iso);
    static Glib::ustring getDisplayStringFromStamp(uint64_t stamp);

  private:
    bool onTimeChanged ();
    static Glib::ustring formatTime(const std::string &format, int64_t diff);
};
