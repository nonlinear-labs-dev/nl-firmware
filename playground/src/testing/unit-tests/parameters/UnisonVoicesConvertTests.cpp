#include <third-party/include/catch.hpp>
#include <parameters/UnisonVoicesParameter.h>
#include "testing/TestHelper.h"

TEST_CASE("Convert Single to Dual Sound Changes Unison Accordingly")
{
  auto eb = TestHelper::getEditBuffer();
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();

  eb->undoableConvertToSingle(transaction, VoiceGroup::I);
  eb->undoableInitSound(transaction);

  auto unisonVoices = eb->findParameterByID({ 249, VoiceGroup::I });
  unisonVoices->setCPFromHwui(scope->getTransaction(), 0);
  REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");

  WHEN("Unison Voices: 1")
  {
    unisonVoices->setCPFromHwui(transaction, 0);  // -> 1
    REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");

    WHEN("Converted to Dual")
    {
      eb->undoableConvertToDual(transaction, SoundType::Layer);
      REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");
    }
  }

  for(auto steps = 1; steps < 24; steps++)
  {
    auto voices = steps + 1;

    WHEN("Unison Voices: " + std::to_string(voices))
    {
      unisonVoices->setCPFromHwui(transaction, 0);
      unisonVoices->stepCPFromHwui(transaction, steps, {});
      REQUIRE(unisonVoices->getDisplayString() == std::to_string(voices) + " voices");

      eb->undoableConvertToDual(transaction, SoundType::Layer);

      if(voices >= 12)
      {
        CHECK(unisonVoices->getDisplayString() == "12 voices");
      }
      else
      {
        CHECK(unisonVoices->getDisplayString() == std::to_string(voices) + " voices");
      }
    }
  }
}

TEST_CASE("Convert Dual to Single Sound")
{
  auto eb = TestHelper::getEditBuffer();
  auto scope = TestHelper::createTestScope();
  auto transaction = scope->getTransaction();

  eb->undoableConvertToDual(transaction, SoundType::Layer);
  eb->undoableInitSound(transaction);

  auto unisonVoices = eb->findParameterByID({ 249, VoiceGroup::I });
  unisonVoices->setCPFromHwui(scope->getTransaction(), 0);
  REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");

  WHEN("Unison Voices: 1")
  {
    unisonVoices->setCPFromHwui(transaction, 0);  // -> 1
    REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");

    WHEN("Converted to Single")
    {
      eb->undoableConvertToSingle(transaction, VoiceGroup::I);
      REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");
    }
  }

  for(auto steps = 1; steps < 12; steps++)
  {
    auto voices = steps + 1;

    WHEN("Unison Voices: " + std::to_string(voices))
    {
      unisonVoices->setCPFromHwui(transaction, 0);
      unisonVoices->stepCPFromHwui(transaction, steps, {});
      REQUIRE(unisonVoices->getDisplayString() == std::to_string(voices) + " voices");

      eb->undoableConvertToSingle(transaction, VoiceGroup::I);
      
      CHECK(unisonVoices->getDisplayString() == std::to_string(voices) + " voices");
    }
  }
}