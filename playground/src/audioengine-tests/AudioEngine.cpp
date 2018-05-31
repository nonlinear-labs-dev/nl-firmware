#include "AudioEngine.h"
#include <stdint.h>
#include <testing/TestDriver.h>
#include <chrono>
#include <device-settings/DebugLevel.h>

namespace AudioEngine
{
  static constexpr auto numVoices = 20;
  static constexpr int numSlots = (numVoices + 1) * 2;

  using ValueType = float;

  struct UnmodulateableParameter
  {
      ValueType value;
  };

  struct ModulateableParameter
  {
      ValueType value;
      ValueType modAmount;
      int32_t modSrc = 0;
  };

  struct IncomingMonophoneParameters
  {
      static constexpr int numUnmodulateableMonophoneParameters = 50;
      static constexpr int numModulateableMonophoneParameters = 20;

      UnmodulateableParameter unmodulateableParameters[numUnmodulateableMonophoneParameters];
      ModulateableParameter modulateableParameters[numModulateableMonophoneParameters];
  };

  struct IncomingPolyphoneParameters
  {
      static constexpr int numUnmodulateablePolyphoneParameters = 100;
      static constexpr int numModulateablePolyphoneParameters = 70;

      UnmodulateableParameter unmodulateableParameters[numUnmodulateablePolyphoneParameters];
      ModulateableParameter modulateableParameters[numModulateablePolyphoneParameters];
  };

  struct IncomingAudioParameters
  {
      IncomingMonophoneParameters m0;
      IncomingMonophoneParameters m1;
      IncomingPolyphoneParameters p0;
      IncomingPolyphoneParameters p1;
  };

  struct MCAppliedMonophone
  {
      static constexpr int numParameters = 70;
      UnmodulateableParameter parameters[numParameters];
  };

  struct MCAppliedPolyphone
  {
      static constexpr int numParameters = 170;
      UnmodulateableParameter parameters[numParameters];
  };

  using PTAppliedMonophone = MCAppliedMonophone;
  using PTAppliedPolyphone = MCAppliedPolyphone;

  using MMAppliedMonophone = PTAppliedMonophone;
  using MMAppliedPolyphone = PTAppliedPolyphone;

  using PMAppliedMonophone = MMAppliedMonophone;
  using PMAppliedPolyphone = MMAppliedPolyphone;

  using SmoothedMonophone = PMAppliedMonophone;
  using SmoothedPolyphone = PMAppliedPolyphone;

  struct MCAppliedAudioParameters
  {
      MCAppliedMonophone m0;
      MCAppliedMonophone m1;
      MCAppliedPolyphone p0;
      MCAppliedPolyphone p1;
  };

  struct PTAppliedAudioParameters
  {
      PTAppliedMonophone m0;
      PTAppliedMonophone m1;
      PTAppliedPolyphone p0;
      PTAppliedPolyphone p1;
  };

  struct MMAppliedAudioParameters
  {
      MMAppliedMonophone m;
      MMAppliedPolyphone p0;
      MMAppliedPolyphone p1;
  };

  struct PMAppliedAudioParameters
  {
      PMAppliedMonophone m;
      PMAppliedPolyphone p;
  };

  struct SmoothedAudioParameters
  {
      SmoothedMonophone m;
      SmoothedPolyphone p;
  };

  struct Voice
  {
      volatile ValueType renderers[PMAppliedMonophone::numParameters + PMAppliedPolyphone::numParameters];

      void doAudio(const MMAppliedAudioParameters &in)
      {
        const PMAppliedAudioParameters pmApplied = doPM(in);
        const SmoothedAudioParameters smoothed = doSmoothing(pmApplied);
        sendToRenderer(smoothed);
      }

      PMAppliedAudioParameters doPM(const MMAppliedAudioParameters &in)
      {
        PMAppliedAudioParameters r;
        r.m = in.m;

        ValueType leftAmount = r.m.parameters[12].value;
        ValueType rightAmount = 1.0f - leftAmount;

        for(int i = 0; i < PMAppliedPolyphone::numParameters; i++)
          r.p.parameters[i].value = leftAmount * in.p0.parameters[i].value + rightAmount * in.p1.parameters[i].value;

        return r;
      }

      SmoothedAudioParameters doSmoothing(const PMAppliedAudioParameters &in)
      {
        SmoothedAudioParameters r;
        r.m = in.m;
        r.p = in.p;
        return r;
      }

      void sendToRenderer(const SmoothedAudioParameters &in)
      {
        for(int i = 0; i < SmoothedMonophone::numParameters; i++)
          renderers[i] = in.m.parameters[i].value;

        for(int i = 0; i < SmoothedPolyphone::numParameters; i++)
          renderers[i + SmoothedMonophone::numParameters] = in.p.parameters[i].value;
      }
  };

  struct VoiceGroup
  {
      IncomingAudioParameters *incomingLeft;
      IncomingAudioParameters *incomingRight;

      MCAppliedAudioParameters mcAppliedLeft;
      MCAppliedAudioParameters mcAppliedRight;

      ValueType presetTransitionPosition = 0;
      ValueType monophonicMorphPosition = 0;

      PTAppliedAudioParameters ptApplied;
      MMAppliedAudioParameters mmApplied;

      Voice *activeVoices[numVoices] = { };

      void doAudio()
      {
        doMC(mcAppliedLeft, incomingLeft);
        doMC(mcAppliedRight, incomingRight);
        doPT();
        doMM();

        doVoices();
      }

      void doMC(MCAppliedAudioParameters& target, const IncomingAudioParameters * src)
      {
        auto tgtM0 = target.m0.parameters;

        for(auto &a : src->m0.unmodulateableParameters)
          (++tgtM0)->value = a.value;

        for(auto &a : src->m0.modulateableParameters)
          (++tgtM0)->value = a.value;

        auto tgtM1 = target.m1.parameters;

        for(auto &a : src->m1.unmodulateableParameters)
          (++tgtM1)->value = a.value;

        for(auto &a : src->m1.modulateableParameters)
          (++tgtM0)->value = a.value;

        // apply modulation here
      }

      void doPT()
      {
        const ValueType leftAmount = 1.0f - presetTransitionPosition;
        const ValueType rightAmount = presetTransitionPosition;

        for(int i = 0; i < MCAppliedMonophone::numParameters; i++)
        {
          ptApplied.m0.parameters[i].value = leftAmount * mcAppliedLeft.m0.parameters[i].value
              + rightAmount * mcAppliedRight.m0.parameters[i].value;
          ptApplied.m1.parameters[i].value = leftAmount * mcAppliedLeft.m1.parameters[i].value
              + rightAmount * mcAppliedRight.m1.parameters[i].value;
        }

        for(int i = 0; i < MCAppliedPolyphone::numParameters; i++)
        {
          ptApplied.p0.parameters[i].value = leftAmount * mcAppliedLeft.p0.parameters[i].value
              + rightAmount * mcAppliedRight.p0.parameters[i].value;
          ptApplied.p1.parameters[i].value = leftAmount * mcAppliedLeft.p1.parameters[i].value
              + rightAmount * mcAppliedRight.p1.parameters[i].value;
        }
      }

      void doMM()
      {
        mmApplied.p0 = ptApplied.p0;
        mmApplied.p1 = ptApplied.p1;

        const ValueType leftAmount = 1.0f - monophonicMorphPosition;
        const ValueType rightAmount = monophonicMorphPosition;

        for(int i = 0; i < PTAppliedMonophone::numParameters; i++)
        {
          mmApplied.m.parameters[i].value = leftAmount * ptApplied.m0.parameters[i].value + rightAmount * ptApplied.m1.parameters[i].value;
        }
      }

      void doVoices()
      {
        for(Voice *v : activeVoices)
          if(v)
            v->doAudio(mmApplied);
      }
  };

  Voice voices[numVoices];
  VoiceGroup voiceGroups[numVoices];
  VoiceGroup *activeVoiceGroups[numVoices];
  IncomingAudioParameters slots[numSlots];

  void doVoiceGroups()
  {
    for(VoiceGroup *g : activeVoiceGroups)
    {
      g->doAudio();
    }
  }

  struct Tester
  {
      static void registerTests()
      {
        g_test_add_func("/AudioEngine/performance", []()
        {
          doPerformanceTest();
        });
      }

      static void doPerformanceTest()
      {
        for(int i = 0; i < numVoices; i++)
        {
          activeVoiceGroups[i] = &(voiceGroups[i]);
          activeVoiceGroups[i]->activeVoices[0] = &(voices[i]);
          activeVoiceGroups[i]->incomingLeft = &(slots[g_random_int_range(0, numSlots)]);
          activeVoiceGroups[i]->incomingRight = &(slots[g_random_int_range(0, numSlots)]);
        }

        constexpr auto sampleRate = 44100;
        constexpr auto numSeconds = 10;
        const int numSamples = sampleRate * numSeconds;

        auto start = std::chrono::high_resolution_clock::now();

        for(int i = 0; i < numSamples; i++)
        {
          doVoiceGroups();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        auto msPerSecond = ms / numSeconds;
        DebugLevel::warning("AudioEngine used up", msPerSecond, "ms per second, which is", 1000.0 / msPerSecond, "x real time.");
      }
  };

  static TestDriver<Tester> tester;
}
