#include "FlacRecorderVirgin.h"
#include <device-settings/Settings.h>
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>

FlacRecorderVirgin::FlacRecorderVirgin(Settings &s)
    : BooleanSetting(s, true)
{
  using namespace nltools::msg;
  m_connection = receive<Recorder::State>(EndPoint::Playground, [&](auto msg) {
    auto isVirgin = get();

    if(msg.recording and isVirgin)
      set(BooleanSettings::BOOLEAN_SETTING_FALSE);
  });
}
