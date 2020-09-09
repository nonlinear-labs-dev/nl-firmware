#include "Setting.h"
#include "Settings.h"
#include <xml/Writer.h>
#include <xml/Attribute.h>

Setting::Setting(UpdateDocumentContributor &parent)
    : super(&parent)
{
}

Setting::~Setting() = default;

bool Setting::persistent() const
{
  return true;
}

sigc::connection Setting::onChange(sigc::slot<void, const Setting *> slot)
{
  return m_signal.connectAndInit(slot, this);
}

void Setting::notify()
{
  m_signal.send(this);
  super::onChange();

  if(static_cast<Settings *>(getParent())->isLoading())
    sendToPlaycontroller(SendReason::SettingsLoaded);
  else
    sendToPlaycontroller(SendReason::SettingChanged);
}

void Setting::writeDocument(Writer &writer, tUpdateID knownRevision) const
{
  bool changed = knownRevision < getUpdateIDOfLastChange();
  writer.writeTextElement("value", save(), Attribute("changed", changed));
}

void Setting::sendToPlaycontroller(SendReason reason) const
{
}

void Setting::setSetting(Initiator initiator, const Glib::ustring &text)
{
    load(text, initiator);
}
