#include "MockDSPHosts.h"
#include <catch.hpp>

void MockDSPHost::onHWChanged(const uint32_t id, float value)
{
}

void MockDSPHost::onKeyDown(const int note, float velocity, InputEvent from)
{
}

void MockDSPHost::onKeyUp(const int note, float velocity, InputEvent from)
{
}

C15::Properties::HW_Return_Behavior MockDSPHost::getBehaviour(int id)
{
  return C15::Properties::HW_Return_Behavior::Zero;
}

SoundType MockDSPHost::getType()
{
  return SoundType::Single;
}

VoiceGroup MockDSPHost::getSplitPartForKey(int key)
{
  return VoiceGroup::I;
}

void MockDSPHost::onKeyDownSplit(const int note, float velocity, VoiceGroup part, DSPInterface::InputEvent from)
{
}

void MockDSPHost::onKeyUpSplit(const int note, float velocity, VoiceGroup part, DSPInterface::InputEvent from)
{
}

PassOnKeyDownHost::PassOnKeyDownHost(const int expectedNote, float expectedVelo, VoiceGroup expectedPart)
    : m_note { expectedNote }
    , m_vel { expectedVelo }
    , m_part { expectedPart }
{
}

void PassOnKeyDownHost::onKeyDown(const int note, float velocity, InputEvent from)
{
  CHECK(m_note == note);
  CHECK(m_vel == velocity);
  m_receivedKeyDown = true;
}

bool PassOnKeyDownHost::didReceiveKeyDown() const
{
  return m_receivedKeyDown;
}

PassOnKeyUpHost::PassOnKeyUpHost(const int expectedNote, float expectedVelo, VoiceGroup expectedPart)
    : m_note { expectedNote }
    , m_vel { expectedVelo }
    , m_part { expectedPart }
{
}

void PassOnKeyUpHost::onKeyUp(const int note, float velocity, InputEvent from)
{
  CHECK(m_note == note);
  CHECK(m_vel == velocity);
  m_receivedKeyUp = true;
}

bool PassOnKeyUpHost::didReceiveKeyUp() const
{
  return m_receivedKeyUp;
}

PassOnHWReceived::PassOnHWReceived(int expectedId, float expectedValue)
    : m_id { expectedId }
    , m_value { expectedValue }
{
}

void PassOnHWReceived::onHWChanged(const uint32_t id, float value)
{
  CHECK(m_id == id);
  CHECK(m_value == value);
  m_receivedHW = true;
}

bool PassOnHWReceived::didReceiveHW() const
{
  return m_receivedHW;
}