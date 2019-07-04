#include <io/files/FileIOReceiver.h>
#include <io/FromEncoderBridge.h>
#include <io/network/WebSocketSender.h>
#include <Application.h>
#include <Options.h>
#include <string.h>

FromEncoderBridge::FromEncoderBridge()
    : Bridge(new WebSocketSender(getDomain()), new FileIOReceiver("/dev/espi_encoder", 1))
{
}

Domain FromEncoderBridge::getDomain()
{
  return Domain::Rotary;
}

void FromEncoderBridge::transmit(Receiver::tMessage msg)
{
  gsize numBytes = 0;
  auto data = static_cast<const int8_t*>(msg->get_data(numBytes));
  sendRotary(data[0]);
}

void FromEncoderBridge::sendRotary(int8_t inc)
{
  scheduleSimpleEvent(inc);
}

void FromEncoderBridge::scheduleSimpleEvent(int8_t inc)
{
  auto msg = Glib::Bytes::create(&inc, 1);
  m_sender->send(msg);
}
