#include <io/files/FileIOReceiver.h>
#include <io/FromEncoderBridge.h>
#include <io/network/WebSocketSender.h>
#include <Application.h>
#include <Options.h>

FromEncoderBridge::FromEncoderBridge()
    : Bridge(new WebSocketSender(getDomain()), new FileIOReceiver("/dev/espi_encoder", 1))
    , m_throttler(std::chrono::milliseconds(5))
{
  Glib::MainContext::get_default()->signal_timeout().connect(
      [=]() {
        static bool upDown = true;
        sendRotary(upDown ? 1 : -1);
        upDown = !upDown;
        return true;
      },
      20);
}

FromEncoderBridge::~FromEncoderBridge()
{
}

Domain FromEncoderBridge::getDomain()
{
  return Application::get().getOptions()->doTimeStamps() ? Domain::TimeStampedRotary : Domain::Rotary;
}

void FromEncoderBridge::sendRotary(int8_t inc)
{
  m_accumulated += inc;

  if(Application::get().getOptions()->doTimeStamps())
  {
    scheduleTimestampedEvent();
  }
  else
  {
    scheduleSimpleEvent();
  }
}

void FromEncoderBridge::scheduleTimestampedEvent()
{
  if(m_firstPendingEventTime == std::chrono::system_clock::time_point::min())
    m_firstPendingEventTime = std::chrono::system_clock::now();

  m_throttler.doTask([this]() {
    if(auto acc = std::exchange(m_accumulated, 0))
    {
      auto timeStamp = std::exchange(m_firstPendingEventTime, std::chrono::system_clock::time_point::min());
      int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeStamp.time_since_epoch()).count();
      uint8_t data[9];
      memcpy(data, &ms, 8);
      memcpy(data + 8, &acc, 1);
      auto msg = Glib::Bytes::create(data, 9);
      m_sender->send(msg);
    }
  });
}

void FromEncoderBridge::scheduleSimpleEvent()
{
  m_throttler.doTask([this]() {
    if(auto acc = std::exchange(m_accumulated, 0))
    {
      auto msg = Glib::Bytes::create(&acc, 1);
      m_sender->send(msg);
    }
  });
}
