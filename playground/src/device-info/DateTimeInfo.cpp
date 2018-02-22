#include <Application.h>
#include <device-info/DateTimeInfo.h>
#include <device-settings/DateTimeAdjustment.h>
#include <device-settings/Settings.h>
#include <xml/Writer.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <tools/TimeTools.h>

DateTimeInfo::DateTimeInfo (DeviceInformation *parent) :
    DeviceInformationItem (parent)
{
  onTimeChanged ();
  Application::get ().getMainContext ()->signal_timeout ().connect_seconds (sigc::mem_fun (this, &DateTimeInfo::onTimeChanged), 60);
}

bool DateTimeInfo::onTimeChanged ()
{
  onChange ();
  return true;
}

void DateTimeInfo::writeDocument (Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement ("date-time", get ());
}

Glib::ustring DateTimeInfo::formatTime(const std::string &format, int64_t diff)
{
  std::stringstream ss;
  std::time_t t = std::time (nullptr) + diff;
  std::tm tm = *std::localtime (&t);
  ss << std::put_time (&tm, format.c_str ());
  return ss.str ();
}

[[deprecated]] Glib::ustring DateTimeInfo::get () const
{
  return formatTime("%FT%T%z", Application::get().getSettings()->getSetting<DateTimeAdjustment>()->get());
}

[[deprecated]] Glib::ustring DateTimeInfo::getIsoStringOfNow()
{
  return formatTime("%FT%T%z", 0);
}


[[deprecated]] Glib::ustring DateTimeInfo::getDisplayString () const
{
  return formatTime("%x %X", Application::get().getSettings()->getSetting<DateTimeAdjustment>()->get());
}

