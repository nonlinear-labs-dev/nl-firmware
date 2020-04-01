#include <catch.hpp>
#include "Toolbox.h"
#include "AudioEngineOptions.h"
#include <synth/C15Synth.h>
#include <synth/c15-audio-engine/dsp_host_dual.h>

namespace Tests
{
  SCENARIO("Voices remain on preset change")
  {
    using namespace std::chrono_literals;

    auto options = createEmptyAudioEngineOptions();
    auto synth = std::make_unique<C15Synth>(options.get());
    loadTestPreset(synth.get(), "voices-remain-on-preset-load", "1ed29aea-5382-4ab5-b04c-ab853699a1dd");

    GIVEN("some notes are played without glitch supression")
    {
      synth->getDsp()->onSettingGlitchSuppr(false);
      synth->simulateKeyDown(50);
      synth->simulateKeyDown(55);
      synth->simulateKeyDown(60);

      synth->measurePerformance(250ms);

      WHEN("preset 643fe46e-6f9f-4919-82f0-27a6a3aae902 is loaded")
      {
        loadTestPreset(synth.get(), "voices-remain-on-preset-load", "643fe46e-6f9f-4919-82f0-27a6a3aae902");
        synth->measurePerformance(250ms);

        THEN("sound is still playing")
        {
          auto result = synth->measurePerformance(250ms);
          REQUIRE(getMaxLevel(std::get<0>(result)) > 0.25f);
        }
      }

      WHEN("preset b590cb36-af0e-4b3f-9088-7a4aca603198 is loaded")
      {
        loadTestPreset(synth.get(), "voices-remain-on-preset-load", "b590cb36-af0e-4b3f-9088-7a4aca603198");
        synth->measurePerformance(250ms);

        THEN("sound is still playing")
        {
          auto result = synth->measurePerformance(250ms);
          REQUIRE(getMaxLevel(std::get<0>(result)) > 0.25f);
        }
      }
    }

    GIVEN("some notes are played with glitch supression")
    {
      synth->getDsp()->onSettingGlitchSuppr(true);
      synth->simulateKeyDown(50);
      synth->simulateKeyDown(55);
      synth->simulateKeyDown(60);

      synth->measurePerformance(250ms);

      WHEN("preset 643fe46e-6f9f-4919-82f0-27a6a3aae902 is loaded")
      {
        loadTestPreset(synth.get(), "voices-remain-on-preset-load", "643fe46e-6f9f-4919-82f0-27a6a3aae902");
        synth->measurePerformance(250ms);

        THEN("sound is still playing")
        {
          auto result = synth->measurePerformance(250ms);
          REQUIRE(getMaxLevel(std::get<0>(result)) > 0.25f);
        }
      }

      WHEN("preset b590cb36-af0e-4b3f-9088-7a4aca603198 is loaded")
      {
        loadTestPreset(synth.get(), "voices-remain-on-preset-load", "b590cb36-af0e-4b3f-9088-7a4aca603198");
        synth->measurePerformance(250ms);

        THEN("sound is still playing")
        {
          auto result = synth->measurePerformance(250ms);
          REQUIRE(getMaxLevel(std::get<0>(result)) > 0.25f);
        }
      }
    }
  }
}
