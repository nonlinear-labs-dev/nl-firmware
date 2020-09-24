#include "SplitPointSyncParameters.h"
#include <device-settings/Settings.h>

SplitPointSyncParameters::SplitPointSyncParameters(Settings& s)
    : Setting(s)
{
}

void SplitPointSyncParameters::load(const Glib::ustring& text, Initiator initiator)
{
  m_state = text == "on";
  notify();
}

Glib::ustring SplitPointSyncParameters::save() const
{
  return m_state ? "on" : "off";
}

Glib::ustring SplitPointSyncParameters::getDisplayString() const
{
  return m_state ? "Active" : "Disabled";
}

bool SplitPointSyncParameters::persistent() const
{
  return false;
}

bool SplitPointSyncParameters::getState() const
{
  return m_state;
}

void SplitPointSyncParameters::setState(bool s)
{
  m_state = s;
  notify();
}
