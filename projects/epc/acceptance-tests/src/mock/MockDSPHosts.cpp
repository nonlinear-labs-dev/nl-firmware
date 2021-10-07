#include "MockDSPHosts.h"
#include <catch.hpp>

void MockDSPHost::onHWChanged(HardwareSource id, float value, bool)
{
}

void MockDSPHost::onKeyDown(const int note, float velocity, InputEventSource from)
{
}

void MockDSPHost::onKeyUp(const int note, float velocity, InputEventSource from)
{
}

void MockDSPHost::onMidiSettingsReceived()
{
}

C15::Properties::HW_Return_Behavior MockDSPHost::getBehaviour(HardwareSource id)
{
  return C15::Properties::HW_Return_Behavior::Zero;
}

SoundType MockDSPHost::getType()
{
  return m_type;
}

VoiceGroup MockDSPHost::getSplitPartForKeyDown(int key)
{
  return VoiceGroup::I;
}

VoiceGroup MockDSPHost::getSplitPartForKeyUp(int key, InputEventSource from)
{
  return VoiceGroup::I;
}

void MockDSPHost::registerNonLocalSplitKeyAssignment(const int note, VoiceGroup part, InputEventSource from)
{
}

void MockDSPHost::unregisterNonLocalSplitKeyAssignment(const int note, VoiceGroup part, InputEventSource from)
{
}

void MockDSPHost::onKeyDownSplit(const int note, float velocity, VoiceGroup part, DSPInterface::InputEventSource from)
{
}

void MockDSPHost::onKeyUpSplit(const int note, float velocity, VoiceGroup part, DSPInterface::InputEventSource from)
{
}

void MockDSPHost::setType(SoundType type)
{
  m_type = type;
}

PassOnKeyDownHost::PassOnKeyDownHost(const int expectedNote, float expectedVelo, VoiceGroup expectedPart)
    : m_note{ expectedNote }
    , m_vel{ expectedVelo }
    , m_part{ expectedPart }
{
}

void PassOnKeyDownHost::onKeyDown(const int note, float velocity, InputEventSource from)
{
  CHECK(m_note == note);
  CHECK(m_vel == velocity);
  m_receivedKeyDown = true;
}

void PassOnKeyDownHost::onKeyDownSplit(const int note, float velocity, VoiceGroup part,
                                       DSPInterface::InputEventSource from)
{
  CHECK(m_note == note);
  CHECK(m_vel == velocity);
  m_receivedKeyDown = true;
}

bool PassOnKeyDownHost::didReceiveKeyDown() const
{
  return m_receivedKeyDown;
}

VoiceGroup PassOnKeyDownHost::getSplitPartForKeyDown(int key)
{
  return VoiceGroup::Global;
}

VoiceGroup PassOnKeyDownHost::getSplitPartForKeyUp(int key, InputEventSource from)
{
  return VoiceGroup::Global;
}

PassOnKeyUpHost::PassOnKeyUpHost(const int expectedNote, float expectedVelo, VoiceGroup expectedPart)
    : m_note{ expectedNote }
    , m_vel{ expectedVelo }
    , m_part{ expectedPart }
{
}

VoiceGroup PassOnKeyUpHost::getSplitPartForKeyDown(int key)
{
  return VoiceGroup::Global;
}

VoiceGroup PassOnKeyUpHost::getSplitPartForKeyUp(int key, InputEventSource from)
{
  return VoiceGroup::Global;
}

void PassOnKeyUpHost::onKeyUp(const int note, float velocity, InputEventSource from)
{
  CHECK(m_note == note);
  CHECK(m_vel == velocity);
  m_receivedKeyUp = true;
}

void PassOnKeyUpHost::onKeyUpSplit(const int note, float velocity, VoiceGroup part, DSPInterface::InputEventSource from)
{
  CHECK(m_note == note);
  CHECK(m_vel == velocity);
  m_receivedKeyUp = true;
}

bool PassOnKeyUpHost::didReceiveKeyUp() const
{
  return m_receivedKeyUp;
}

PassOnHWReceived::PassOnHWReceived(HardwareSource expectedId, float expectedValue)
    : m_id { expectedId }
    , m_value { expectedValue }
{
}

void PassOnHWReceived::onHWChanged(HardwareSource id, float value, bool)
{
  CHECK(m_id == id);
  CHECK(m_value == value);
  m_receivedHW = true;
}

bool PassOnHWReceived::didReceiveHW() const
{
  return m_receivedHW;
}

void PassOnHWReceived::setExpectedHW(HardwareSource hw)
{
  m_id = hw;
}

void ConfigureableDSPHost::onHWChanged(HardwareSource id, float value, bool behaviourChanged)
{
  if(m_onHWChanged)
    m_onHWChanged(id, value, behaviourChanged);
}

void ConfigureableDSPHost::onKeyDown(const int note, float velocity, DSPInterface::InputEventSource from)
{
  if(m_onKeyDown)
    m_onKeyDown(note, velocity, from);
}

void ConfigureableDSPHost::onKeyUp(const int note, float velocity, DSPInterface::InputEventSource from)
{
  if(m_onKeyUp)
    m_onKeyUp(note, velocity, from);
}

C15::Properties::HW_Return_Behavior ConfigureableDSPHost::getBehaviour(HardwareSource id)
{
  if(m_getBehaviour)
    return m_getBehaviour(id);
  return C15::Properties::HW_Return_Behavior::Stay;
}

VoiceGroup ConfigureableDSPHost::getSplitPartForKeyDown(int key)
{
  if(m_hardcodedSplitKey.has_value())
  {
    return key > m_hardcodedSplitKey.value() ? VoiceGroup::II : VoiceGroup::I;
  }

  if(m_getSplitPartForKey)
    return m_getSplitPartForKey(key);

  return VoiceGroup::I;
}

VoiceGroup ConfigureableDSPHost::getSplitPartForKeyUp(int key, InputEventSource from)
{
  if(m_hardcodedSplitKey.has_value())
  {
    return key > m_hardcodedSplitKey.value() ? VoiceGroup::II : VoiceGroup::I;
  }

  // not sure here...
  if(m_getSplitPartForKey)
    return m_getSplitPartForKey(key);

  return VoiceGroup::I;
}

void ConfigureableDSPHost::onKeyDownSplit(const int note, float velocity, VoiceGroup part,
                                          DSPInterface::InputEventSource from)
{
  if(m_onKeyDownSplit)
    m_onKeyDownSplit(note, velocity, part, from);
}
void ConfigureableDSPHost::onKeyUpSplit(const int note, float velocity, VoiceGroup part,
                                        DSPInterface::InputEventSource from)
{
  if(m_onKeyUpSplit)
    m_onKeyUpSplit(note, velocity, part, from);
}

void ConfigureableDSPHost::setOnHWChangedCB(std::function<void(HardwareSource, float, bool)>&& cb)
{
  m_onHWChanged = cb;
}

void ConfigureableDSPHost::setGetSplitPartForKeyCB(std::function<VoiceGroup(int)>&& cb)
{
  m_getSplitPartForKey = cb;
}

void ConfigureableDSPHost::setOnKeyDownSplitCB(std::function<void(int, float, VoiceGroup, InputEventSource)>&& cb)
{
  m_onKeyDownSplit = cb;
}

void ConfigureableDSPHost::setGetBehaviourCB(std::function<C15::Properties::HW_Return_Behavior(HardwareSource)>&& cb)
{
  m_getBehaviour = cb;
}

void ConfigureableDSPHost::setOnKeyUpCB(std::function<void(int, float, InputEventSource)>&& cb)
{
  m_onKeyUp = cb;
}

void ConfigureableDSPHost::setOnKeyDownCB(std::function<void(int, float, InputEventSource)>&& cb)
{
  m_onKeyDown = cb;
}

void ConfigureableDSPHost::setOnKeyUpSplitCB(std::function<void(int, float, VoiceGroup, InputEventSource)>&& cb)
{
  m_onKeyUpSplit = cb;
}

void ConfigureableDSPHost::setSplitPointKey(int i)
{
  m_hardcodedSplitKey = i;
}
