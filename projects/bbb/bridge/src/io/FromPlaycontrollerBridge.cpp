#include <io/FromPlaycontrollerBridge.h>
#include "files/PlaycontrollerReceiver.h"
#include <nltools/messaging/Message.h>

FromPlaycontrollerBridge::FromPlaycontrollerBridge()
    : Bridge(nullptr, new PlaycontrollerReceiver())
{
}

void FromPlaycontrollerBridge::sendRibbonPosition(bool m_upperRibon, double value)
{
  gint16 messageType = 1024;
  gint16 ribbonId = static_cast<gint16>(m_upperRibon ? 6 : 7);
  gint16 val = static_cast<gint16>(value);

  gint16 data[4];
  data[0] = messageType;
  data[1] = 2;
  data[2] = ribbonId;
  data[3] = val;
  transmit(Glib::Bytes::create(data, 8));
}

void FromPlaycontrollerBridge::transmit(const Receiver::tMessage &i)
{
  nltools::msg::PlaycontrollerMessage msg;
  msg.message = i;
  nltools::msg::send(nltools::msg::EndPoint::Playground, msg);
}
