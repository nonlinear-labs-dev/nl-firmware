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
#include <glibmm/main.h>

DateTimeInfo::DateTimeInfo(DeviceInformation *parent)
    : DeviceInformationItem(parent)
{
  onTimeChanged();
  m_signalMinutePassed = Application::get().getMainContext()->signal_timeout().connect_seconds(
      sigc::mem_fun(this, &DateTimeInfo::onTimeChanged), 60);
}

DateTimeInfo::~DateTimeInfo()
{
  m_signalMinutePassed.disconnect();
}

bool DateTimeInfo::onTimeChanged()
{
  onChange(UpdateDocumentContributor::ChangeFlags::Generic);
  return true;
}

void DateTimeInfo::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("date-time", get());
  writer.writeTextElement("date-time-display", getDisplayString());
}

Glib::ustring DateTimeInfo::get() const
{
  return TimeTools::getAdjustedIso();
}

Glib::ustring DateTimeInfo::getDisplayString() const
{
  return TimeTools::getDisplayStringFromIso(TimeTools::getAdjustedIso());
}
