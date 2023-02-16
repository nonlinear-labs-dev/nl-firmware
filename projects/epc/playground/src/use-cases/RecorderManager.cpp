#include "RecorderManager.h"
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>

RecorderManager::RecorderManager()
{
  using tMsg = nltools::msg::Setting::NotifyNoRecorderClients;
  nltools::msg::receive<tMsg>(nltools::msg::EndPoint::Playground, [this](tMsg msg) { m_signal.deferedSend(); });
}

sigc::connection RecorderManager::subscribeToNotifyNoRecorderUIsLeftAndStillPlaying(sigc::slot<void(void)> s)
{
  return m_signal.connect(s);
}

void RecorderManager::stopRecorderPlayback()
{
  nltools::msg::Setting::FlacRecorderStopPlayback msg {};
  nltools::msg::send(nltools::msg::EndPoint::AudioEngine, msg);
}
