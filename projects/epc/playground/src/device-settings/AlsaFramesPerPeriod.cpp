#include "AlsaFramesPerPeriod.h"
#include "Settings.h"
#include <nltools/logging/Log.h>
#include <nltools/messaging/Message.h>
#include <nltools/messaging/Messaging.h>

AlsaFramesPerPeriod::AlsaFramesPerPeriod(Settings &parent)
    : super(parent)
{
}

AlsaFramesPerPeriod::~AlsaFramesPerPeriod() = default;

void AlsaFramesPerPeriod::load(const Glib::ustring &text, Initiator initiator)
{
  try
  {
    set(std::stoi(text));
  }
  catch(...)
  {
    set(0);
    nltools::Log::error("Could not read settings for frames per period:", text);
  }
}

void AlsaFramesPerPeriod::loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val)
{
  auto f = std::get<float>(val);
  set(static_cast<int>(f));
}

Glib::ustring AlsaFramesPerPeriod::save() const
{
  return to_string(m_framesPerPeriod);
}

void AlsaFramesPerPeriod::set(int fpp)
{
  fpp = std::clamp(fpp, 1, 512);

  if(std::exchange(m_framesPerPeriod, fpp) != fpp)
    notify();
}

void AlsaFramesPerPeriod::syncExternals(SendReason reason) const
{
  nltools::msg::SetFramesPerPeriod msg { m_framesPerPeriod };
  nltools::msg::send(nltools::msg::EndPoint::AudioEngine, msg);
}

Glib::ustring AlsaFramesPerPeriod::getDisplayString() const
{
  return std::to_string(m_framesPerPeriod);
}
