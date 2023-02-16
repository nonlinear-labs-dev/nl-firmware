#include <catch.hpp>
#include <parameters/UnisonVoicesParameter.h>
#include "testing/TestHelper.h"
#include <libundo/undo/Scope.h>
#include <http/UndoScope.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Convert Single to Dual Sound Changes Unison Accordingly",
                 "[Unison][Parameter]")
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCase(*eb);
  TestHelper::initSingleEditBuffer();

  auto unisonVoices = eb->findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::I });
  ParameterUseCases unisonUseCases(unisonVoices);

  WHEN("Unison Voices: 1")
  {
    unisonUseCases.setControlPosition(0);
    REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");

    WHEN("Converted to Dual")
    {
      ebUseCase.convertToLayer(VoiceGroup::I);
      REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");
    }
  }

  SECTION("Steps")
  {
    auto steps = GENERATE(range(1, 23));
    auto voices = steps + 1;

    WHEN("Unison Voices: " + std::to_string(voices))
    {
      unisonUseCases.setControlPosition(0);
      unisonUseCases.stepControlPosition(steps, false, false);
      REQUIRE(unisonVoices->getDisplayString() == std::to_string(voices) + " voices");

      ebUseCase.convertToLayer(VoiceGroup::I);

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

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Convert Dual to Single Sound", "[Unison][Parameter]")
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);
  useCase.convertToLayer(VoiceGroup::I);
  useCase.initSound(Defaults::FactoryDefault);

  auto unisonVoices = eb->findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::I });
  ParameterUseCases unisonUseCase(unisonVoices);

  WHEN("Unison Voices: 1")
  {
    unisonUseCase.setControlPosition(0);
    REQUIRE(unisonVoices->getDisplayString() == "1 voice (off)");

    WHEN("Converted to Single")
    {
      useCase.convertToSingle(VoiceGroup::I);
      CHECK(unisonVoices->getDisplayString() == "1 voice (off)");
    }
  }

  SECTION("Steps")
  {
    auto steps = GENERATE(range(1, 11));
    auto voices = steps + 1;

    WHEN("Unison Voices: " + std::to_string(voices))
    {
      unisonUseCase.setControlPosition(0);
      unisonUseCase.stepControlPosition(steps, false, false);
      REQUIRE(unisonVoices->getDisplayString() == std::to_string(voices) + " voices");

      useCase.convertToSingle(VoiceGroup::I);
      CHECK(unisonVoices->getDisplayString() == std::to_string(voices) + " voices");
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Undo Convert Sound resets Scaling", "[Unison][Parameter]")
{
  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases useCase(*eb);
  auto unisonVoicesI = eb->findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::I });
  auto unisonVoicesII = eb->findParameterByID({ C15::PID::Unison_Voices, VoiceGroup::II });
  ParameterUseCases unisonI(unisonVoicesI);
  ParameterUseCases unisonII(unisonVoicesII);

  SECTION("Single -> Dual")
  {
    useCase.convertToSingle(VoiceGroup::I);
    REQUIRE(eb->getType() == SoundType::Single);
    REQUIRE(unisonVoicesI->getValue().getCoarseDenominator() == 23);
    REQUIRE(unisonVoicesII->getValue().getCoarseDenominator() == 23);

    useCase.convertToLayer(VoiceGroup::I);

    REQUIRE(eb->getType() == SoundType::Layer);
    REQUIRE(unisonVoicesI->getValue().getCoarseDenominator() == 11);
    REQUIRE(unisonVoicesII->getValue().getCoarseDenominator() == 11);

    THEN("Values Match")
    {
      unisonI.setControlPosition(1);
      unisonII.setControlPosition(1);
      REQUIRE(unisonVoicesI->getDisplayString() == "12 voices");
      REQUIRE(unisonVoicesII->getDisplayString() == "12 voices");

      unisonI.setControlPosition(0);
      unisonII.setControlPosition(0);
      REQUIRE(unisonVoicesI->getDisplayString() == "1 voice (off)");
      REQUIRE(unisonVoicesII->getDisplayString() == "1 voice (off)");
    }

    WHEN("Undo")
    {
      REQUIRE(eb->getType() == SoundType::Layer);
      eb->getParent()->getUndoScope().undo();
      REQUIRE(eb->getType() == SoundType::Single);

      REQUIRE(unisonVoicesI->getValue().getCoarseDenominator() == 23);
      REQUIRE(unisonVoicesII->getValue().getCoarseDenominator() == 23);

      THEN("Values Match")
      {
        unisonI.setControlPosition(1);
        unisonII.setControlPosition(1);
        REQUIRE(unisonVoicesI->getDisplayString() == "24 voices");
        REQUIRE(unisonVoicesII->getDisplayString() == "24 voices");

        unisonI.setControlPosition(0);
        unisonII.setControlPosition(0);
        REQUIRE(unisonVoicesI->getDisplayString() == "1 voice (off)");
        REQUIRE(unisonVoicesII->getDisplayString() == "1 voice (off)");
      }
    }
  }

  SECTION("Dual -> Single")
  {
    useCase.convertToLayer(VoiceGroup::I);

    REQUIRE(eb->getType() == SoundType::Layer);
    REQUIRE(unisonVoicesI->getValue().getCoarseDenominator() == 11);
    REQUIRE(unisonVoicesII->getValue().getCoarseDenominator() == 11);

    THEN("convert to single")
    {
      useCase.convertToSingle(VoiceGroup::I);

      REQUIRE(eb->getType() == SoundType::Single);
      REQUIRE(unisonVoicesI->getValue().getCoarseDenominator() == 23);
      REQUIRE(unisonVoicesII->getValue().getCoarseDenominator() == 23);

      THEN("Values Match")
      {
        unisonI.setControlPosition(1);
        unisonII.setControlPosition(1);
        REQUIRE(unisonVoicesI->getDisplayString() == "24 voices");
        REQUIRE(unisonVoicesII->getDisplayString() == "24 voices");

        unisonI.setControlPosition(0);
        unisonII.setControlPosition(0);
        REQUIRE(unisonVoicesI->getDisplayString() == "1 voice (off)");
        REQUIRE(unisonVoicesII->getDisplayString() == "1 voice (off)");
      }

      WHEN("UNDONE")
      {
        REQUIRE(eb->getType() == SoundType::Single);
        eb->getParent()->getUndoScope().undo();
        REQUIRE(eb->getType() == SoundType::Layer);

        REQUIRE(unisonVoicesI->getValue().getCoarseDenominator() == 11);
        REQUIRE(unisonVoicesII->getValue().getCoarseDenominator() == 11);

        THEN("Values match")
        {
          unisonI.setControlPosition(1);
          unisonII.setControlPosition(1);
          REQUIRE(unisonVoicesI->getDisplayString() == "12 voices");
          REQUIRE(unisonVoicesII->getDisplayString() == "12 voices");

          unisonI.setControlPosition(0);
          unisonII.setControlPosition(0);
          REQUIRE(unisonVoicesI->getDisplayString() == "1 voice (off)");
          REQUIRE(unisonVoicesII->getDisplayString() == "1 voice (off)");
        }
      }
    }
  }
}
