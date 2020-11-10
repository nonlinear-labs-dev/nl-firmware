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
  return m_state ? "On" : "Off";
}

bool SplitPointSyncParameters::get() const
{
  return m_state;
}

void SplitPointSyncParameters::setState(bool s)
{
  m_state = s;
  notify();
}

void SplitPointSyncParameters::setState(bool s, UNDO::Transaction* t)
{
  m_state = s;
  m_signalWithTransaction.emit(this, t);
  UpdateDocumentContributor::onChange(ChangeFlags::Generic);
}

sigc::connection SplitPointSyncParameters::onChangeWithTransaction(const tSlot& cb)
{
  return m_signalWithTransaction.connect(cb);
}
