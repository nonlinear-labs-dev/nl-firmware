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

  WHEN("Unison Voices: 14")
  {
    unisonVoices->setCPFromHwui(transaction, 0.583333338);  // -> 14
    REQUIRE(unisonVoices->getDisplayString() == "14 voices");

    WHEN("Convert to Dual")
    {
      eb->undoableConvertToDual(transaction, SoundType::Layer);
      REQUIRE(unisonVoices->getDisplayString() == "12 voices");
    }
  }

  WHEN("Unison Voices: 2")
  {
    unisonVoices->stepCPFromHwui(transaction, 1, {});  // -> 2
    REQUIRE(unisonVoices->getDisplayString() == "2 voices");

    WHEN("Converted to Dual")
    {
      eb->undoableConvertToDual(transaction, SoundType::Layer);
      REQUIRE(unisonVoices->getDisplayString() == "2 voices");
    }
  }

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
}

TEST_CASE("Convert Dual to Single Sound")
{
}