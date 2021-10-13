#include <Application.h>
#include "UniqueHardwareID.h"
#include <proxies/playcontroller/PlaycontrollerProxy.h>

UniqueHardwareID::UniqueHardwareID(DeviceInformation* p)
    : DeviceInformationItem(p)
{
  m_connection = Application::get().getPlaycontrollerProxy()->onUHIDChanged(
      sigc::mem_fun(this, &UniqueHardwareID::onIDReceived));
}

Glib::ustring UniqueHardwareID::get() const
{
  if(m_id == 0)
    return "0000-0000-0000-0000";

  using i16 = uint16_t;
  const i16 p0 = m_id >> 48;
  const i16 p1 = m_id >> 32;
  const i16 p2 = m_id >> 16;
  const i16 p3 = m_id;

  std::stringstream ss;
  ss << std::hex << std::uppercase << std::internal << std::setfill('0') << std::setw(4) << p0 << "-" << std::hex
     << std::uppercase << std::internal << std::setfill('0') << std::setw(4) << p1 << "-" << std::hex << std::uppercase
     << std::internal << std::setfill('0') << std::setw(4) << p2 << "-" << std::hex << std::uppercase << std::internal
     << std::setfill('0') << std::setw(4) << p3;
  return ss.str();
}

void UniqueHardwareID::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("unique-hardware-id", get());
}

void UniqueHardwareID::onIDReceived(uint64_t id)
{
  m_id = id;
  DeviceInformationItem::onChange();
}
