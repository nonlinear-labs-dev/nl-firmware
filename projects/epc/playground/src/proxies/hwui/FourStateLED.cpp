#include <glib.h>
#include "FourStateLED.h"
#include "device-settings/DebugLevel.h"
#include <Application.h>
#include <nltools/messaging/Message.h>

FourStateLED::FourStateLED()
    : m_syncThrottler(Application::get().getMainContext(), std::chrono::milliseconds(5))
{
}

FourStateLED::~FourStateLED()
{
  setState(State::Bright, false);
}

void FourStateLED::setState(State state, bool throttle)
{
  if(m_state != state)
  {
    m_state = state;
    syncHWUI(throttle);
  }
}

void FourStateLED::syncHWUI(bool throttle)
{
  using namespace nltools::msg;
  SetRibbonLEDMessage msg {};
  msg.id = static_cast<uint8_t>(getID());
  msg.brightness = static_cast<uint8_t>(m_state & 0x03);

  if(throttle)
  {
    m_syncThrottler.doTask([msg]() { send(EndPoint::RibbonLed, msg); });
  }
  else
  {
    send(EndPoint::RibbonLed, msg);
  }
}

FourStateLED::State FourStateLED::getState() const
{
  return m_state;
}
