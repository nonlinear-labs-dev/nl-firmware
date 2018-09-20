#include <io/framebuffer/FrameBufferSender.h>
#include <io/network/WebSocketReceiver.h>
#include <io/ToOledsBridge.h>

#include <Application.h>
#include <Options.h>

ToOledsBridge::ToOledsBridge()
    : super(new FrameBufferSender(), new WebSocketReceiver(Domain::Oled))
    , m_timeStampedFramebufferReceiver(std::make_unique<WebSocketReceiver>(Domain::TimeStampedOled))
{
  m_timeStampedFramebufferReceiver->setCallback([this](auto msg) { handleTimeStampedFramebuffer(msg); });

  if(Application::get().getOptions()->doTimeStamps())
    Glib::MainContext::get_default()->signal_timeout().connect_seconds(
        sigc::mem_fun(this, &ToOledsBridge::printTurnaroundTime), 1);
}

ToOledsBridge::~ToOledsBridge()
{
}

void ToOledsBridge::handleTimeStampedFramebuffer(Receiver::tMessage msg)
{
  gsize numBytes = 0;
  auto bytes = reinterpret_cast<const int8_t*>(msg->get_data(numBytes));
  int64_t ms = 0;
  memcpy(&ms, bytes, 8);
  auto fb = Glib::Bytes::create(bytes + 8, numBytes - 8);
  m_sender->send(fb);

  if(ms)
  {
    auto now = std::chrono::system_clock::now();
    int64_t nowMS = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    auto diff = nowMS - ms;

    m_timeStamps.push_back({ now, diff });
  }
}

void ToOledsBridge::removeOldRecords()
{
  auto now = std::chrono::system_clock::now();

  auto e = std::remove_if(m_timeStamps.begin(), m_timeStamps.end(), [&](auto p) {
    auto age = now - p.captureTime;

    return age > std::chrono::seconds(5);
  });

  m_timeStamps.erase(e, m_timeStamps.end());
}

bool ToOledsBridge::printTurnaroundTime()
{
  removeOldRecords();

  auto maxIt = std::max_element(m_timeStamps.begin(), m_timeStamps.end(), [](auto a, auto b) { return a.ms < b.ms; });

  if(maxIt != m_timeStamps.end())
  {
    TRACE("Max turn around time in past 5 seconds: " << maxIt->ms << "ms.");
  }
  else
  {
    TRACE("No encoder event in past 5 seconds.");
  }

  return true;
}
