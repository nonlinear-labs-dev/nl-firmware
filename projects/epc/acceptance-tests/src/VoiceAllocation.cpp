#include <catch.hpp>
#include "Toolbox.h"
#include "AudioEngineOptions.h"
#include <synth/C15Synth.h>
#include <synth/c15-audio-engine/dsp_host_dual.h>
#include <mock/DspHostDualTester.h>

namespace Tests
{
  TEST_CASE("Initial State")
  {
    using namespace std::chrono_literals;
    auto options = createEmptyAudioEngineOptions();
    auto synth = std::make_unique<C15Synth>(options.get());
    DspHostDualTester tester{ synth->getDsp() };
    THEN("No Active Voices")
    {
      CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
    }
    THEN("Full Polyphony")
    {
      CHECK(tester.getAssignableVoices() == C15::Config::total_polyphony);
    }
    THEN("Silence")
    {
      const auto output = synth->measurePerformance(20ms);
      CHECK(tester.scanOutput(std::get<0>(output)).isSilence());
    }
  }
  TEST_CASE("Key Association")
  {
    using namespace std::chrono_literals;
    auto options = createEmptyAudioEngineOptions();
    auto synth = std::make_unique<C15Synth>(options.get());
    DspHostDualTester tester{ synth->getDsp() };
    GIVEN("TCD KeyDown")
    {
      tester.applyTCDKeyDown(60, 1.0f, VoiceGroup::Global);
      synth->measurePerformance(20ms);
      const auto voices = tester.getActiveVoices(VoiceGroup::Global);
      THEN("One Active Voice")
      {
        CHECK(voices == 1);
      }
      WHEN("Same TCD KeyDown")
      {
        tester.applyTCDKeyDown(60, 1.0f, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("Active Voices unchanged")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
        }
      }
      WHEN("Same TCD KeyUp")
      {
        tester.applyTCDKeyUp(60, 1.0f, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("No Active Voices")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
      }
      WHEN("Different TCD KeyDown")
      {
        tester.applyTCDKeyDown(61, 1.0f, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("Active Voices increased")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) > voices);
        }
      }
      WHEN("Unpressed TCD KeyUp")
      {
        tester.applyTCDKeyUp(61, 1.0f, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("Active Voices unchanged")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
        }
      }
      WHEN("Same MIDI NoteOn")
      {
        tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("Active Voices increased")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) > voices);
        }
        WHEN("Same MIDI NoteOff")
        {
          tester.applyMidiNoteOff(60, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
          synth->measurePerformance(20ms);
          THEN("Active Voices decreased")
          {
            CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
          }
        }
      }
    }
    GIVEN("MIDI NoteOn")
    {
      tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
      synth->measurePerformance(20ms);
      const auto voices = tester.getActiveVoices(VoiceGroup::Global);
      THEN("One Active Voice")
      {
        CHECK(voices == 1);
      }
      WHEN("Same MIDI NoteOn")
      {
        tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("Active Voices unchanged")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
        }
      }
      WHEN("Same MIDI NoteOff")
      {
        tester.applyMidiNoteOff(60, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("No Active Voices")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
      }
      WHEN("Different MIDI NoteOn")
      {
        tester.applyMidiNoteOn(61, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("Active Voices increased")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) > voices);
        }
      }
      WHEN("Unpressed MIDI NoteOff")
      {
        tester.applyMidiNoteOff(61, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("Active Voices unchanged")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
        }
      }
      WHEN("Same TCD KeyDown")
      {
        tester.applyTCDKeyDown(60, 1.0f, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("Active Voices increased")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) > voices);
        }
        WHEN("Same TCD KeyUp")
        {
          tester.applyTCDKeyUp(60, 1.0f, VoiceGroup::Global);
          synth->measurePerformance(20ms);
          THEN("Active Voices decreased")
          {
            CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
          }
        }
      }
    }
    GIVEN("Distinguish Internal/Primary/Secondary Keys")
    {
      WHEN("Single Sound")
      {
        tester.applyMalformedSinglePreset({ 1, Polyphony::Poly });
        synth->measurePerformance(20ms);
        WHEN("TCD Key Down")
        {
          tester.applyTCDKeyDown(60, 1.0f, VoiceGroup::Global);
          synth->measurePerformance(20ms);
          THEN("One Active Voice")
          {
            CHECK(tester.getActiveVoices(VoiceGroup::Global) == 1);
          }
          WHEN("Same MIDI Primary NoteOn")
          {
            tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
            synth->measurePerformance(20ms);
            THEN("Two Active Voices")
            {
              CHECK(tester.getActiveVoices(VoiceGroup::Global) == 2);
            }
            WHEN("Same MIDI Secondary NoteOn")
            {
              tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Secondary, VoiceGroup::Global);
              synth->measurePerformance(20ms);
              THEN("Active Voices unchanged")
              {
                CHECK(tester.getActiveVoices(VoiceGroup::Global) == 2);
              }
            }
          }
        }
      }
      WHEN("Split Sound")
      {
        tester.applyMalformedSplitPreset({ 1, Polyphony::Poly }, { 1, Polyphony::Poly });
        synth->measurePerformance(20ms);
        WHEN("TCD Key Down")
        {
          tester.applyTCDKeyDown(60, 1.0f, VoiceGroup::I);
          synth->measurePerformance(20ms);
          THEN("One Active Voice")
          {
            CHECK(tester.getActiveVoices(VoiceGroup::Global) == 1);
          }
          WHEN("Same MIDI Primary NoteOn")
          {
            tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Primary, VoiceGroup::I);
            synth->measurePerformance(20ms);
            THEN("Two Active Voices")
            {
              CHECK(tester.getActiveVoices(VoiceGroup::Global) == 2);
            }
            WHEN("Same MIDI Secondary NoteOn")
            {
              tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Secondary, VoiceGroup::II);
              synth->measurePerformance(20ms);
              THEN("Three Active Voices")
              {
                CHECK(tester.getActiveVoices(VoiceGroup::Global) == 3);
              }
            }
          }
        }
      }
      WHEN("Layer Sound")
      {
        tester.applyMalformedLayerPreset({ 1, Polyphony::Poly });
        synth->measurePerformance(20ms);
        WHEN("TCD Key Down")
        {
          tester.applyTCDKeyDown(60, 1.0f, VoiceGroup::Global);
          synth->measurePerformance(20ms);
          THEN("One Active Voice in Part I")
          {
            CHECK(tester.getActiveVoices(VoiceGroup::I) == 1);
          }
          THEN("One Active Voice in Part II")
          {
            CHECK(tester.getActiveVoices(VoiceGroup::II) == 1);
          }
          WHEN("Same MIDI Primary NoteOn")
          {
            tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
            synth->measurePerformance(20ms);
            THEN("Two Active Voices in Part I")
            {
              CHECK(tester.getActiveVoices(VoiceGroup::I) == 2);
            }
            THEN("Two Active Voices in Part II")
            {
              CHECK(tester.getActiveVoices(VoiceGroup::II) == 2);
            }
            WHEN("Same MIDI Secondary NoteOn")
            {
              tester.applyMidiNoteOn(60, 1.0f, MockInputEventSource::Secondary, VoiceGroup::Global);
              synth->measurePerformance(20ms);
              THEN("Active Voices unchanged in Part I")
              {
                CHECK(tester.getActiveVoices(VoiceGroup::I) == 2);
              }
              THEN("Active Voices unchanged in Part II")
              {
                CHECK(tester.getActiveVoices(VoiceGroup::II) == 2);
              }
            }
          }
        }
      }
    }
  }
  TEST_CASE("128 Keys")
  {
    using namespace std::chrono_literals;
    auto options = createEmptyAudioEngineOptions();
    auto synth = std::make_unique<C15Synth>(options.get());
    DspHostDualTester tester{ synth->getDsp() };
    GIVEN("Lowest TCD KeyDown")
    {
      tester.applyTCDKeyDown(0, 1.0f, VoiceGroup::Global);
      synth->measurePerformance(20ms);
      THEN("One Active Voice")
      {
        CHECK(tester.getActiveVoices(VoiceGroup::Global) == 1);
      }
      WHEN("Lowest TCD KeyUp")
      {
        tester.applyTCDKeyUp(0, 1.0f, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("No Active Voices")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
      }
    }
    GIVEN("Highest TCD KeyDown")
    {
      tester.applyTCDKeyDown(127, 1.0f, VoiceGroup::Global);
      synth->measurePerformance(20ms);
      THEN("One Active Voice")
      {
        CHECK(tester.getActiveVoices(VoiceGroup::Global) == 1);
      }
      WHEN("Highest TCD KeyUp")
      {
        tester.applyTCDKeyUp(127, 1.0f, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("No Active Voices")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
      }
    }
    GIVEN("Lowest MIDI NoteOn")
    {
      tester.applyMidiNoteOn(0, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
      synth->measurePerformance(20ms);
      THEN("One Active Voice")
      {
        CHECK(tester.getActiveVoices(VoiceGroup::Global) == 1);
      }
      WHEN("Lowest MIDI NoteOff")
      {
        tester.applyMidiNoteOff(0, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("No Active Voices")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
      }
    }
    GIVEN("Highest MIDI NoteOn")
    {
      tester.applyMidiNoteOn(127, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
      synth->measurePerformance(20ms);
      THEN("One Active Voice")
      {
        CHECK(tester.getActiveVoices(VoiceGroup::Global) == 1);
      }
      WHEN("Highest MIDI NoteOff")
      {
        tester.applyMidiNoteOff(127, 1.0f, MockInputEventSource::Primary, VoiceGroup::Global);
        synth->measurePerformance(20ms);
        THEN("No Active Voices")
        {
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
      }
    }
  }
  // TODO: replace
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
          tester.applyMonoMessage(Polyphony::Mono, VoiceGroup::Global);
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("unison voices changed: reset?")
        {
          tester.applyUnisonMessage(2, VoiceGroup::Global);
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
        WHEN("synth reset: reset?")
        {
          synth->resetDSP();
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
        tester.applyMalformedSinglePreset({ 1, Polyphony::Mono });
        // wait a bit
        synth->measurePerformance(20ms);
        // are available voices correct? (single is total polyphony - 24 voices)
        CHECK(tester.getAssignableVoices() == C15::Config::total_polyphony);
        // press a key
        tester.applyTCDKeyDown(60, 1.0f, VoiceGroup::Global);
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
          tester.applyMalformedSinglePreset({ 1, Polyphony::Mono });
          // wait a bit
          synth->measurePerformance(20ms);
          // active voices should not have changed
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == voices);
        }
        WHEN("same sound type, mono changed: reset?")
        {
          tester.applyMalformedSinglePreset({ 1, Polyphony::Poly });
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("same sound type, unison changed: reset?")
        {
          tester.applyMalformedSinglePreset({ 2, Polyphony::Mono });
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("single -> split: reset?")
        {
          tester.applyMalformedSplitPreset({ 1, Polyphony::Mono }, { 1, Polyphony::Mono });
          // wait a bit
          synth->measurePerformance(20ms);
          // no more voice should be active
          CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
        }
        WHEN("single -> layer: reset?")
        {
          tester.applyMalformedLayerPreset({ 1, Polyphony::Mono });
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

    // TODO: remove

    // ???
    //    auto preset1 = "dcc9b3d3-009d-4363-a64d-930c95d435a5";
    //    auto preset2 = "119284ae-b3d4-42a7-a155-31f04ed340ac";

    // ??? could not detect full 24 voices here, preset seems to be split or layer
    //    loadTestPreset(synth.get(), "voices-remain-on-preset-load", preset1);
  }
}
