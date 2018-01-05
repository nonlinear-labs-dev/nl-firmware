#include "DeviceInformationItem.h"

DeviceInformationItem::DeviceInformationItem (DeviceInformation *parent) :
    UpdateDocumentContributor (parent)
{
}

DeviceInformationItem::tUpdateID DeviceInformationItem::onChange ()
{
  auto ret = UpdateDocumentContributor::onChange ();
  m_signal.send (this);
  return ret;
}

Glib::ustring DeviceInformationItem::getDisplayString() const
{
	return get();
}

connection DeviceInformationItem::onChange (slot<void, const DeviceInformationItem*> slot)
{
  return m_signal.connectAndInit (slot, this);
}

