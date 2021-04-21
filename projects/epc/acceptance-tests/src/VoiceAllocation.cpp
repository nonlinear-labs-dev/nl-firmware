#include <catch.hpp>
#include "Toolbox.h"
#include "AudioEngineOptions.h"
#include <synth/C15Synth.h>
#include <synth/c15-audio-engine/dsp_host_dual.h>
#include <mock/DspHostDualTester.h>

namespace Tests
{
  // description doesn't really fit at the moment..
  TEST_CASE("Voices remain on preset change")
  {
    using namespace std::chrono_literals;
    auto options = createEmptyAudioEngineOptions();
    auto synth = std::make_unique<C15Synth>(options.get());
    DspHostDualTester tester{ synth->getDsp() };
    //Prepare Runtime Options for MIDI
    {
      nltools::msg::Setting::MidiSettingsMessage msg;
      msg.receiveNotes = true;
      msg.receiveChannel = MidiReceiveChannel::CH_1;
      synth->onMidiSettingsMessage(msg);
    }
    // initial state checks...
    GIVEN("initial state: no active voices? full polyphony?")
    {
      // are ALL voices (Part I, II) inactive?
      CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
      // are available voices correct? (initially full/total polyphony - 24 voices)
      CHECK(tester.getAssignableVoices() == C15::Config::total_polyphony);
    }
    GIVEN("initial state: keypress/release?")
    {
      WHEN("press a key: active voices?")
      {
        // press a key
        synth->doMidi({ 0x90, 0x50, 0x7F });
        // wait a bit
        synth->measurePerformance(20ms);
        // latch voices for subsequent comparison
        const auto voices = tester.getActiveVoices(VoiceGroup::Global);
        // is a voice active?
        CHECK(voices > 0);
        // subsequent key checks...
        WHEN("press same key same: voices unchanged?")
        {
          synth->doMidi({ 0x90, 0x50, 0x7F });
          // wait a bit
          synth->measurePerformance(20ms);
          // active voices should not have changed
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
        }
        WHEN("release pressed key: active voices?")
        {
          synth->doMidi({ 0x80, 0x50, 0x7F });
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("release unpressed key: voices unchanged?")
        {
          synth->doMidi({ 0x80, 0x60, 0x7F });
          // wait a bit
          synth->measurePerformance(20ms);
          // active voices should not have changed
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
        }
        // subsequent reset checks...
        WHEN("mono enable changed: reset?")
        {
          tester.sendMonoMessage(true, VoiceGroup::Global);
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("unison voies changed: reset?")
        {
          tester.sendUnisonMessage(2, VoiceGroup::Global);
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("midi setting message: reset?")
        {
          {
            nltools::msg::Setting::MidiSettingsMessage msg;
            msg.receiveNotes = true;
            msg.receiveChannel = MidiReceiveChannel::CH_2;
            synth->onMidiSettingsMessage(msg);
          }
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
      }
    }
    GIVEN("preset messages: reset?")
    {
      WHEN("single preset: reset?")
      {
        // prepare
        tester.sendSinglePreset(false, 1);
        // wait a bit
        synth->measurePerformance(20ms);
        // press a key
        tester.sendTCDKeyDown(60, 1.0f, VoiceGroup::Global);
        // wait a bit
        synth->measurePerformance(20ms);
        // latch voices for subsequent comparison
        const auto voices = tester.getActiveVoices(VoiceGroup::Global);
        // is a voice active?
        CHECK(voices > 0);
        // subsequent preset checks...
        WHEN("same sound type, no mono/unison changed: no reset?")
        {
          // prepare
          tester.sendSinglePreset(false, 1);
          // wait a bit
          synth->measurePerformance(20ms);
          // active voices should not have changed
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
        }
        WHEN("same sound type, mono changed: reset?")
        {
          tester.sendSinglePreset(true, 1);
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("same sound type, unison changed: reset?")
        {
          tester.sendSinglePreset(false, 2);
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("single -> split: reset?")
        {
          tester.sendSplitPreset(false, 1, false, 1);
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("single -> layer: reset?")
        {
          tester.sendLayerPreset(false, 1);
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
      }
      // in a split sound, we could check if same key pressed in primary and secondary would produce changes in assigned voices
      // maybe, rearrange whole thing: single (key checks, reset checks), split (...), layer (...)

      // TODO:
      // - establish TCD KeyDown/Up
      // ... mono/unison parameters, preset, midi settings
      // ... voice stealing, part association ...
      // ... midi channels, internal vs external notes
    }

    // ???
    //    auto preset1 = "dcc9b3d3-009d-4363-a64d-930c95d435a5";
    //    auto preset2 = "119284ae-b3d4-42a7-a155-31f04ed340ac";

    // ??? could not detect full 24 voices here, preset seems to be split or layer
    //    loadTestPreset(synth.get(), "voices-remain-on-preset-load", preset1);

    // deprecated
    /*
    GIVEN("some notes are played without glitch suppression")
    {
      synth->getDsp()->onSettingGlitchSuppr(false);
      synth->doMidi({ 0x90, 0x50, 0x7F });
      synth->doMidi({ 0x90, 0x53, 0x7F });
      synth->doMidi({ 0x90, 0x56, 0x7F });

      synth->measurePerformance(250ms);

      WHEN("preset 2 is loaded")
      {
        loadTestPreset(synth.get(), "voices-remain-on-preset-load", preset2);
        synth->measurePerformance(250ms);

        THEN("sound is still playing")
        {
          auto result = synth->measurePerformance(250ms);
          REQUIRE(getMaxLevel(std::get<0>(result)) > 0.5f);
        }
      }
    }

    GIVEN("some notes are played with glitch suppression")
    {
      synth->getDsp()->onSettingGlitchSuppr(true);
      synth->doMidi({ 0x90, 0x50, 0x7F });
      synth->doMidi({ 0x90, 0x53, 0x7F });
      synth->doMidi({ 0x90, 0x56, 0x7F });

      synth->measurePerformance(250ms);

      WHEN("preset 2 is loaded")
      {
        loadTestPreset(synth.get(), "voices-remain-on-preset-load", preset2);
        synth->measurePerformance(250ms);

        THEN("sound is still playing")
        {
          auto result = synth->measurePerformance(250ms);
          REQUIRE(getMaxLevel(std::get<0>(result)) > 0.5f);
        }
      }
    }
    */
  }
}
