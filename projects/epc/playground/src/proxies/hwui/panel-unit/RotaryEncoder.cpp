#include <Application.h>
#include <device-settings/DebugLevel.h>
#include <device-settings/EncoderAcceleration.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <proxies/hwui/Oleds.h>
#include <tools/PerformanceTimer.h>
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <proxies/hwui/HWUI.h>

RotaryEncoder::RotaryEncoder()
    : m_throttler(Application::get().getMainContext(), std::chrono::milliseconds(2))
{
  nltools::msg::receive<nltools::msg::RotaryChangedMessage>(nltools::msg::EndPoint::Playground,
                                                            sigc::mem_fun(this, &RotaryEncoder::onMessage));

  nltools::msg::receive<nltools::msg::TimestampedRotaryChangedMessage>(
      nltools::msg::EndPoint::Playground, sigc::mem_fun(this, &RotaryEncoder::onMessageTimestamped));
}

RotaryEncoder::~RotaryEncoder()
{
  m_stress.disconnect();
}

void RotaryEncoder::onMessage(const nltools::msg::RotaryChangedMessage &msg)
{
  applyIncrement(msg.increment);
}

void RotaryEncoder::onMessageTimestamped(const nltools::msg::TimestampedRotaryChangedMessage &msg)
{
  bool wasDirty = Application::get().getHWUI()->getOleds().isDirty();
  applyIncrement(msg.increment);
  bool isDirty = Application::get().getHWUI()->getOleds().isDirty();

  if(!wasDirty && isDirty && m_oldestPendingTimestamp == 0)
  {
    m_oldestPendingTimestamp = msg.timestamp;
  }
}

void RotaryEncoder::applyIncrement(tIncrement currentInc)
{
  m_signalRotaryChanged.send(currentInc);

  if((currentInc < 0) != (m_accumulatedIncs < 0))
    m_accumulatedIncs = 0;

  m_accumulatedIncs += currentInc;

  m_throttler.doTask([this]() {
    if(abs(m_accumulatedIncs) > 1)
    {
      m_accumulatedIncs = std::min(m_accumulatedIncs, 10);
      m_accumulatedIncs = std::max(m_accumulatedIncs, -10);
      double factor = Application::get().getSettings()->getSetting<EncoderAcceleration>()->get();
      int sign = m_accumulatedIncs < 0 ? -1 : 1;
      m_signalRotaryChanged.send(factor * sign * m_accumulatedIncs * m_accumulatedIncs);
    }
    m_accumulatedIncs = 0;
  });
}

void RotaryEncoder::fake(tIncrement i)
{
  m_signalRotaryChanged.send(i);
}

sigc::connection RotaryEncoder::onRotaryChanged(sigc::slot<void, tIncrement> slot)
{
  return m_signalRotaryChanged.connect(slot);
}

int64_t RotaryEncoder::resetOldestPendingTimestamp()
{
  return std::exchange(m_oldestPendingTimestamp, 0);
}
