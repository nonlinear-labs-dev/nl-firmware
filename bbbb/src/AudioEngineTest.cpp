#include <stdint.h>
#include <chrono>
#include "bbbb.h"

namespace AudioEngine
{
  static constexpr auto numVoices = 20;
  static constexpr int numSlots = (numVoices + 1) * 2;

  static constexpr int numUnmodulateableMonophonicParameters = 50;
  static constexpr int numModulateableMonophonicParameters = 20;
  static constexpr int numUnmodulateablePolyphonicParameters = 100;
  static constexpr int numModulateablePolyphonicParameters = 70;
  static constexpr int numPolyphonicParameters = numUnmodulateablePolyphonicParameters + numModulateablePolyphonicParameters;
  static constexpr int numMonophonicParameters = numUnmodulateableMonophonicParameters + numModulateableMonophonicParameters;
  static constexpr int numModulateableParameters = numModulateableMonophonicParameters + numModulateablePolyphonicParameters;
  static constexpr int numUnmodulateableParameters = numUnmodulateableMonophonicParameters + numUnmodulateablePolyphonicParameters;
  static constexpr int numParameters = numPolyphonicParameters + numMonophonicParameters;

  using ValueType = float;

  namespace Tools
  {
    template<int numElements>
      inline void interpolate(ValueType *target, const ValueType *left, const ValueType *right, ValueType leftAmount)
      {
        const ValueType rightAmount = 1.0f - leftAmount;

#pragma omp simd aligned(target, left, right:4)
        for(int i = 0; i < numElements; i++)
          target[i] = leftAmount * left[i] + rightAmount * right[i];
      }

    template<typename Target, typename Source>
      inline void morphModulateableParameter(Target &target, const Source &left, const Source &right, ValueType morphPosition)
      {
        const auto leftSign = left.modAmount >= 0 ? 1.0f : -1.0f;
        const auto rightSign = right.modAmount >= 0 ? 1.0f : -1.0f;
        const auto leftY = std::abs(left.modAmount);
        const auto rightY = -std::abs(right.modAmount);
        const auto m = rightY - leftY;
        const auto x = leftY;
        const auto y = m + morphPosition + x;
        target.modSrc = y > 0 ? left.modSrc : right.modSrc;
        target.modAmount = y > 0 ? leftSign * y : rightSign * y;
      }
  }

  namespace Incoming
  {
    using UnmodulateableParameter = ValueType;

    struct ModulateableParameter
    {
        ValueType value = 0;
        ValueType modAmount = 0;
        int32_t modSrc = 0;
    };

    struct MonophoneParameters
    {
        static constexpr int numUnmodulateableParameters = numUnmodulateableMonophonicParameters;
        static constexpr int numModulateableParameters = numModulateableMonophonicParameters;
        static constexpr int numParameters = numModulateableParameters + numUnmodulateableParameters;

        UnmodulateableParameter unmodulateableParameters[numUnmodulateableParameters] = { };
        ModulateableParameter modulateableParameters[numModulateableParameters] = { };
    };

    struct PolyphoneParameters
    {
        static constexpr int numUnmodulateableParameters = numUnmodulateablePolyphonicParameters;
        static constexpr int numModulateableParameters = numModulateablePolyphonicParameters;
        static constexpr int numParameters = numUnmodulateableParameters + numModulateableParameters;

        UnmodulateableParameter unmodulateableParameters[numUnmodulateableParameters] = { };
        ModulateableParameter modulateableParameters[numModulateableParameters] = { };
    };

    struct AudioParameters
    {
        MonophoneParameters m0 = { };
        MonophoneParameters m1 = { };
        PolyphoneParameters p0 = { };
        PolyphoneParameters p1 = { };
    };
  }

  namespace PTApplied
  {
    using UnmodulateableParameter = ValueType;

    struct ModulateableParameter
    {
        ValueType value = 0;
        ValueType modAmount = 0;
        int32_t modSrc = 0;
    };

    struct MonophoneParameters
    {
        static constexpr int numUnmodulateableParameters = numUnmodulateableMonophonicParameters;
        static constexpr int numModulateableParameters = numModulateableMonophonicParameters;
        static constexpr int numParameters = numModulateableParameters + numUnmodulateableParameters;

        UnmodulateableParameter unmodulateableParameters[numUnmodulateableParameters] = { };
        ModulateableParameter modulateableParameters[numModulateableParameters] = { };
    };

    struct PolyphoneParameters
    {
        static constexpr int numUnmodulateableParameters = numUnmodulateablePolyphonicParameters;
        static constexpr int numModulateableParameters = numModulateablePolyphonicParameters;
        static constexpr int numParameters = numUnmodulateableParameters + numModulateableParameters;

        UnmodulateableParameter unmodulateableParameters[numUnmodulateableParameters] = { };
        ModulateableParameter modulateableParameters[numModulateableParameters] = { };
    };

    struct AudioParameters
    {
        MonophoneParameters m0 = { };
        MonophoneParameters m1 = { };
        PolyphoneParameters p0 = { };
        PolyphoneParameters p1 = { };
    };

    void applyPT(AudioParameters &target, const Incoming::AudioParameters& left, const Incoming::AudioParameters& right,
                 ValueType leftAmount)
    {
      Tools::interpolate<MonophoneParameters::numUnmodulateableParameters>(target.m0.unmodulateableParameters,
          left.m0.unmodulateableParameters, right.m0.unmodulateableParameters, leftAmount);

      Tools::interpolate<MonophoneParameters::numUnmodulateableParameters>(target.m1.unmodulateableParameters,
          left.m1.unmodulateableParameters, right.m1.unmodulateableParameters, leftAmount);

      Tools::interpolate<PolyphoneParameters::numUnmodulateableParameters>(target.p0.unmodulateableParameters,
          left.p0.unmodulateableParameters, right.p0.unmodulateableParameters, leftAmount);

      Tools::interpolate<PolyphoneParameters::numUnmodulateableParameters>(target.p1.unmodulateableParameters,
          left.p1.unmodulateableParameters, right.p1.unmodulateableParameters, leftAmount);

      const ValueType rightAmount = 1.0f - leftAmount;

      auto m0ModTgt = target.m0.modulateableParameters;
      const auto m0ModSrcLeft = left.m0.modulateableParameters;
      const auto m0ModSrcRight = right.m0.modulateableParameters;

#pragma omp simd aligned(m0ModTgt, m0ModSrcLeft, m0ModSrcRight:4)
      for(int i = 0; i < MonophoneParameters::numModulateableParameters; i++)
        m0ModTgt[i].value = m0ModSrcLeft[i].value * leftAmount + m0ModSrcRight[i].value * rightAmount;

      for(int i = 0; i < MonophoneParameters::numModulateableParameters; i++)
        Tools::morphModulateableParameter(m0ModTgt[i], m0ModSrcLeft[i], m0ModSrcRight[i], leftAmount);

      auto m1ModTgt = target.m1.modulateableParameters;
      const auto m1ModSrcLeft = left.m1.modulateableParameters;
      const auto m1ModSrcRight = right.m1.modulateableParameters;

#pragma omp simd aligned(m1ModTgt, m1ModSrcLeft, m1ModSrcRight:4)
      for(int i = 0; i < MonophoneParameters::numModulateableParameters; i++)
        m1ModTgt[i].value = m1ModSrcLeft[i].value * leftAmount + m1ModSrcRight[i].value * rightAmount;

      for(int i = 0; i < PolyphoneParameters::numModulateableParameters; i++)
        Tools::morphModulateableParameter(m1ModTgt[i], m1ModSrcLeft[i], m1ModSrcRight[i], leftAmount);
    }
  }

  namespace MMApplied
  {
    using UnmodulateableParameter = ValueType;

    struct ModulateableParameter
    {
        ValueType value = 0;
        ValueType modAmount = 0;
        int32_t modSrc = 0;
    };

    struct MonophoneParameters
    {
        static constexpr int numUnmodulateableParameters = numUnmodulateableMonophonicParameters;
        static constexpr int numModulateableParameters = numModulateableMonophonicParameters;
        static constexpr int numParameters = numModulateableParameters + numUnmodulateableParameters;

        UnmodulateableParameter unmodulateableParameters[numUnmodulateableParameters] = { };
        ModulateableParameter modulateableParameters[numModulateableParameters] = { };
    };

    struct PolyphoneParameters
    {
        static constexpr int numUnmodulateableParameters = numUnmodulateablePolyphonicParameters;
        static constexpr int numModulateableParameters = numModulateablePolyphonicParameters;
        static constexpr int numParameters = numUnmodulateableParameters + numModulateableParameters;

        UnmodulateableParameter unmodulateableParameters[numUnmodulateableParameters] = { };
        ModulateableParameter modulateableParameters[numModulateableParameters] = { };
    };

    struct AudioParameters
    {
        MonophoneParameters m = { };
        PolyphoneParameters p0 = { };
        PolyphoneParameters p1 = { };
    };

    void applyMM(AudioParameters &target, const PTApplied::AudioParameters &in, ValueType morphPosition)
    {
      Tools::interpolate<MonophoneParameters::numUnmodulateableParameters>(target.m.unmodulateableParameters,
          in.m0.unmodulateableParameters, in.m1.unmodulateableParameters, morphPosition);

      for(int i = 0; i < MonophoneParameters::numModulateableParameters; i++)
        Tools::morphModulateableParameter(target.m.modulateableParameters[i], in.m0.modulateableParameters[i],
            in.m1.modulateableParameters[i], morphPosition);
    }
  }

  namespace PMApplied
  {
    using Parameter = ValueType;
    struct AudioParameters
    {
        Parameter parameters[numParameters] = { };
    };

    void applyPM(AudioParameters &target, const MMApplied::AudioParameters &in, ValueType morphPosition)
    {
      Parameter *tgt = target.parameters;

      Tools::interpolate<MMApplied::PolyphoneParameters::numUnmodulateableParameters>(tgt, in.p0.unmodulateableParameters,
          in.p1.unmodulateableParameters, morphPosition);

      tgt += MMApplied::PolyphoneParameters::numUnmodulateableParameters;

      for(int i = 0; i < MMApplied::PolyphoneParameters::numModulateableParameters; i++)
      {
        MMApplied::ModulateableParameter morphedModParam;
        Tools::morphModulateableParameter(morphedModParam, in.p0.modulateableParameters[i], in.p1.modulateableParameters[i], morphPosition);

        //where to calc MC positions?
        //calc morphedModParam modulation and store result in tgt
      }
    }
  }


  struct Voice
  {
      ValueType renderers[numParameters] = { };

      PMApplied::AudioParameters pmApplied = { };
      ValueType morphPosition = 0;

      void doAudio(const MMApplied::AudioParameters &in)
      {
        PMApplied::applyPM(pmApplied, in, morphPosition);
        sendToRenderer(pmApplied);
      }

      void sendToRenderer(const PMApplied::AudioParameters &in)
      {
        for(int i = 0; i < numParameters; i++)
          renderers[i] = in.parameters[i];
      }
  };

  struct VoiceGroup
  {
      Incoming::AudioParameters *incomingLeft = { };
      Incoming::AudioParameters *incomingRight = { };

      PTApplied::AudioParameters ptApplied = { };
      MMApplied::AudioParameters mmApplied = { };

      ValueType presetTransitionPosition = 0;
      ValueType monophonicMorphPosition = 0;

      Voice *activeVoices[numVoices] = { };
      int numVoicesUsed = 0;

      void doAudio()
      {
        PTApplied::applyPT(ptApplied, *incomingLeft, *incomingRight, 1.0f - presetTransitionPosition);
        MMApplied::applyMM(mmApplied, ptApplied, monophonicMorphPosition);
        doVoices(mmApplied);
      }

      void doVoices(const MMApplied::AudioParameters &p)
      {
        for(Voice *v : activeVoices)
          if(v)
            v->doAudio(p);
      }
  };

  Voice voices[numVoices];
  VoiceGroup voiceGroups[numVoices];
  VoiceGroup *activeVoiceGroups[numVoices];
  Incoming::AudioParameters slots[numSlots];

  void doVoiceGroups()
  {
    for(int i = 0; i < numVoices; i++)
    {
      if(activeVoiceGroups[i])
        activeVoiceGroups[i]->doAudio();
    }
  }

  static void doAudioEnginePerformanceTest()
  {
    return;

    int numVoiceGroupsUsed = 20;

    for(int vg = 0; vg < numVoiceGroupsUsed; vg++)
    {
      activeVoiceGroups[vg] = &(voiceGroups[vg]);
      auto g = activeVoiceGroups[vg];
      g->incomingLeft = &(slots[g_random_int_range(0, numSlots)]);
      g->incomingRight = &(slots[g_random_int_range(0, numSlots)]);
      g->presetTransitionPosition = 0.4;
    }

    for(int v = 0; v < numVoices; v++)
    {
      auto g = activeVoiceGroups[g_random_int_range(0, numVoiceGroupsUsed)];
      g->activeVoices[g->numVoicesUsed] = &(voices[v]);
      g->numVoicesUsed++;
    }

    constexpr auto sampleRate = 96000;
    constexpr auto numSeconds = 3600;
    constexpr auto controlRate = 100.0;
    constexpr auto numSamplesPerControlRate = sampleRate / controlRate;
    const int numSamples = sampleRate * numSeconds;

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < numSamples / numSamplesPerControlRate; i++)
    {
      doVoiceGroups();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
    auto nsPerSecond = ns / numSeconds;
    std::cout << "AudioEngine used up " << nsPerSecond << " ns per second, which is " << 1000000000.0 / nsPerSecond << " x real time."
              << std::endl;

    exit(0);
  }

  struct DoTheTest
  {
      DoTheTest()
      {
        doAudioEnginePerformanceTest();
      }
  };

  static DoTheTest theTest;
}
