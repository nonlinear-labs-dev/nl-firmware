#include <io/files/FileIOReceiver.h>
#include <io/FromEncoderBridge.h>
#include <io/network/WebSocketSender.h>

FromEncoderBridge::FromEncoderBridge()
    : Bridge(new WebSocketSender(Domain::Rotary), new FileIOReceiver("/dev/espi_encoder", 1))
    , m_throttler(std::chrono::milliseconds(5))
{
}

FromEncoderBridge::~FromEncoderBridge()
{
}

void FromEncoderBridge::sendRotary(int8_t inc)
{
  m_accumulated += inc;

  m_throttler.doTask([this]() {
    if(auto acc = std::exchange(m_accumulated, 0))
    {
      auto msg = Glib::Bytes::create(g_memdup(&acc, 1), 1);
      m_sender->send(msg);
    }
  });
}
