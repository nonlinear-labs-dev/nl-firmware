#include <device-settings/DeviceName.h>
#include <device-settings/Settings.h>
#include <libundo/undo/StringTools.h>
#include <set>
#include <tools/TimeTools.h>
#include <glibmm.h>
#include "nltools/Assert.h"

DeviceName::DeviceName(Settings &parent)
    : super(parent)
    , m_settings(parent)
{
}

DeviceName::~DeviceName()
{
}

void DeviceName::loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val)
{
  nltools_detailedAssertAlways(false, "this setting is not defaultable");
}

void DeviceName::load(const Glib::ustring &text, Initiator initiator)
{
  set(text);
}

Glib::ustring DeviceName::save() const
{
  return get();
}

void DeviceName::set(Glib::ustring name)
{
  if(m_name != name || m_name.empty())
  {
    m_name = name;
    sanitize(m_settings.getSetting<DateTimeAdjustment>());
    notify();
  }
}

const Glib::ustring &DeviceName::get() const
{
  return m_name;
}

void DeviceName::sanitize(DateTimeAdjustment *adj)
{
  if(m_name.empty())
  {
    guint16 u = TimeTools::getAdjustedTimestamp(adj);

    try
    {
      std::hash<std::string> h;
      std::string out = Glib::file_get_contents("/sys/class/net/wlan0/address");
      u = (guint16) h(out);
    }
    catch(...)
    {
    }

    char txt[256];
    snprintf(txt, 256, "Unit-%05d", u);
    set(txt);
  }
}

Glib::ustring DeviceName::getDisplayString() const
{
  return m_name;
}
