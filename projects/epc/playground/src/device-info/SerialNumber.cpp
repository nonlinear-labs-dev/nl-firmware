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
  if(m_id == 0)
    return "0000-0000-0000-0000";

  using i16 = uint16_t;
  const i16 p0 = m_id >> 48;
  const i16 p1 = m_id >> 32;
  const i16 p2 = m_id >> 16;
  const i16 p3 = m_id;

  std::stringstream ss;
  ss << std::hex << std::uppercase << p0 << "-" << p1 << "-" << p2 << "-" << p3;
  return ss.str();
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
