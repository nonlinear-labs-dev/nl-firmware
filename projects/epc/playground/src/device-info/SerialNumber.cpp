#include <Application.h>
#include "SerialNumber.h"
#include <proxies/playcontroller/PlaycontrollerProxy.h>

SerialNumber::SerialNumber(DeviceInformation* p)
    : DeviceInformationItem(p)
{
  m_connection
      = Application::get().getPlaycontrollerProxy()->onUHIDChanged(sigc::mem_fun(this, &SerialNumber::onIDReceived));
}

Glib::ustring SerialNumber::get() const
{
  return std::to_string(m_id);
}

void SerialNumber::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("serial-number", std::to_string(m_id));
}

void SerialNumber::onIDReceived(uint64_t id)
{
  m_id = id;
  DeviceInformationItem::onChange();
}
