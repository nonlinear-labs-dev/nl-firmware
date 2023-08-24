#include "TestToneTypeSetting.h"
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>

static std::vector<TestToneType> order { TestToneType::Off, TestToneType::Left, TestToneType::Right,
                                         TestToneType::Both };

const std::vector<Glib::ustring> &TestToneTypeSetting::enumToString() const
{
  static auto ret
      = std::vector<Glib::ustring>({ toString(order[0]), toString(order[1]), toString(order[2]), toString(order[3]) });
  return ret;
}

void TestToneTypeSetting::syncExternals(SendReason reason) const
{
  nltools::msg::TestTone::TestToneTypeMessage type;
  nltools::msg::TestTone::TestTonePanMessage pan;

  pan.signalLeft = get() == TestToneType::Both || get() == TestToneType::Left;
  pan.signalRight = get() == TestToneType::Both || get() == TestToneType::Right;
  type.testToneOn = get() != TestToneType::Off;

  constexpr auto endPoint = nltools::msg::EndPoint::AudioEngine;
  nltools::msg::send(endPoint, type);
  nltools::msg::send(endPoint, pan);
}

const std::vector<Glib::ustring> &TestToneTypeSetting::enumToDisplayString() const
{
  static auto ret = std::vector<Glib::ustring>({ "Off", "Left", "Right", "Both" });
  return ret;
}

void TestToneTypeSetting::loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val)
{
  set(std::get<TestToneType>(val));
}

bool TestToneTypeSetting::persistent() const
{
  return false;
}