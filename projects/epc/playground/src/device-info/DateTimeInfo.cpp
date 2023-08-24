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
  auto adj = Application::get().getSettings()->getSetting<DateTimeAdjustment>();
  m_signalAdjustmentChanged
      = adj->onChange(sigc::hide(sigc::mem_fun(this, &DateTimeInfo::onDateTimeAdjustmentChanged)));

  m_signalMinutePassed = Application::get().getMainContext()->signal_timeout().connect_seconds(
      sigc::mem_fun(this, &DateTimeInfo::onTimeChanged), 60);

  onChange(UpdateDocumentContributor::ChangeFlags::Generic);
}

DateTimeInfo::~DateTimeInfo()
{
  m_signalMinutePassed.disconnect();
  m_signalAdjustmentChanged.disconnect();
}

bool DateTimeInfo::onTimeChanged()
{
  onChange(UpdateDocumentContributor::ChangeFlags::Generic);
  return true;
}

void DateTimeInfo::onDateTimeAdjustmentChanged()
{
  onChange(UpdateDocumentContributor::ChangeFlags::Generic);
}

void DateTimeInfo::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("date-time", get());
  writer.writeTextElement("date-time-display", getDisplayString());
}

Glib::ustring DateTimeInfo::get() const
{
  auto adj = Application::get().getSettings()->getSetting<DateTimeAdjustment>();
  return TimeTools::getAdjustedIso(adj);
}

Glib::ustring DateTimeInfo::getDisplayString() const
{
  auto adj = Application::get().getSettings()->getSetting<DateTimeAdjustment>();
  return TimeTools::getDisplayStringFromIso(TimeTools::getAdjustedIso(adj));
}
