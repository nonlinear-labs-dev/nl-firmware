#include "DateTimeAdjustment.h"
#include "Application.h"
#include "device-settings/Settings.h"
#include "proxies/playcontroller/PlaycontrollerProxy.h"
#include "http/UpdateDocumentMaster.h"
#include <device-settings/DebugLevel.h>
#include <boost/lexical_cast.hpp>

DateTimeAdjustment::DateTimeAdjustment(Settings &parent)
    : super(parent)
    , m_adjustment(0)
{
}

void DateTimeAdjustment::loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val)
{
  auto f = std::get<float>(val);
  set(static_cast<int64_t>(f));
}

DateTimeAdjustment::~DateTimeAdjustment()
{
}

void DateTimeAdjustment::load(const Glib::ustring &text, Initiator initiator)
{
  try
  {
    set(boost::lexical_cast<int64_t>(text.raw()));
  }
  catch(...)
  {
    set(0);
    DebugLevel::error("Could not read settings for date time adjustment:", text);
  }
}

Glib::ustring DateTimeAdjustment::save() const
{
  return to_string(get());
}

void DateTimeAdjustment::set(int64_t adjustment)
{
  if(m_adjustment != adjustment)
  {
    m_adjustment = adjustment;
    notify();
  }
}

int64_t DateTimeAdjustment::get() const
{
  return m_adjustment;
}

void DateTimeAdjustment::adjust(time_t modifiedTime)
{
  set(modifiedTime - std::time(nullptr));
}

Glib::ustring DateTimeAdjustment::getDisplayString() const
{
  return std::to_string(get());
}
