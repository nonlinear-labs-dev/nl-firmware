#pragma once

class C15
{

    // MonoMorph
    // PolyMorph
    // Split
    // Layer
    // Seamless Preset Change
    // MacroControls
    // VoiceAlloc
    // ParamEdit
    // VoiceFree
    // Dualphone
    // Monophone
    // Polyphone

    class AudioParameterArray
    {
        static const int NUM_PARAMETERS = 324;
        int16_t allTheParameters[NUM_PARAMETERS];
    };

    class MorphableAudioParameters
    {
        static const int NUM_MORPH_SOURCES = 2;
        AudioParameterArray morph[NUM_MORPH_SOURCES];
    };

    class Voice
    {
        MorphableAudioParameters *currentParameters; // pointing
        AudioParameterArray morphResult;
        double morphPosition;

        int note;

        void doMorph()
        {
          interpolate(morphResult, currentParameters, morphPosition);
        }

        void applyModulation()
        {
        }

        void noteOff()
        {
        }

        bool isFinished()
        {
          return false;
        }
    };

    class VoiceGroup
    {
        std::list<Voice> activeVoices;

        // points to previously selected preset
        int previousSlot = 0;

        // points to currently selected preset - parameters in this slot will be edited by UI
        int currentSlot = 0;

        // 0 ... 1 => used for interpolation during preset transition
        double presetTransition = 1.0;

        // increment determining the transition speed
        double presetTransitionIncrement = 0.01;

        // result for interpolation during preset transition
        MorphableAudioParameters presetTransitionBuffer;

        bool getPresetTransitionSetting()
        {
          // return true if presets should be morphed into each other
          return false;
        }

        int findFreeSlot()
        {
          // iterate all active voices and find a this.slot[] that is not used
          return 0;
        }

        void perInstrument()
        {
          doPresetTransition();
        }

        void doPresetTransition()
        {
          if(presetTransition < 1)
          { // interpolation in progress
            presetTransition += presetTransitionIncrement;

            interpolate(presetTransitionBuffer, slots[previousSlot], slots[currentSlot], presetTransition);

            if(presetTransition >= 1)
              finishPresetTransition();
          }
        }

        void finishPresetTransition()
        {
          letAllActiveVoicesPointTo(slots[currentSlot]);
        }

        void letAllActiveVoicesPointTo(MorphableAudioParameters parameters)
        {
          for(Voice v : activeVoices)
            v.parameters = parameters;
        }

        void interpolate(MorphableAudioParameters target, MorphableAudioParameters from, MorphableAudioParameters to, double pos)
        {
        }

        void perVoice()
        {
          for(Voice v : activeVoices)
          {
            v.doMorph();
            v.applyModulation();
          }
        }

        void add(Voice v)
        {
          v.parameters = slots[currentSlot];
          activeVoices.add(v);
        }

        void noteOff(int key)
        {
          for(Voice v : activeVoices)
          {
            if(v.key == key)
              v.noteOff();
          }
        }

        void freeFinishedVoices()
        {
          for(Voice &v : activeVoices)
          {
            if(v.isFinished())
            {
              activeVoices.remove(v);
              voicePool.add(v);
            }
          }
        }

        void perSample()
        {
          perInstrument();
          perVoice();
        }

        void loadPreset(MorphableAudioParameters params)
        {
          // load the preset into a new slot
          int s = findFreeSlot();
          slots[s] = params;

          previousSlot = currentSlot;
          currentSlot = s;

          if(getPresetTransitionSetting())
          {
            // start transition with next 'perSample'-call
            presetTransition = 0;
            presetTransitionBuffer = slots[previousSlot];
            letAllActiveVoicesPointTo(presetTransitionBuffer);
          }
          else
          {
            // current voices should stay at its parameters
            // new voices will use parameters in 'currentSlot'
          }
        }
    };

    static const int NUM_VOICES = 20;
    static const int NUM_VOICE_GROUPS = 2;

    std::list<Voice> voicePool;
    MorphableAudioParameters slots[NUM_VOICES + 1];
    VoiceGroup groups[NUM_VOICE_GROUPS];
    int splitKey = 60;

    enum Mode
    {
      NORMAL, SPLIT, LAYER
    };

    Mode mode = Mode::NORMAL;

    void noteOn(int key)
    {
      freeFinishedVoices();

      switch(mode)
      {
        case Mode::NORMAL:
          allocSingleVoice (group[0]);
          break;

        case Mode::SPLIT:
          allocSplitVoice(key);
          break;

        case Mode::LAYER:
          allocLayeredVoices();
          break;
      }
    }

    void noteOff(int key)
    {
      for(VoiceGroup &g : groups)
        g.noteOff(key);
    }

    void loadPreset(MorphableAudioParameters params, int whichGroup)
    {
      groups[whichGroup].loadPreset(params);
    }

    void perSample()
    {
      for(VoiceGroup &g : groups)
        g.perSample();
    }

    void freeFinishedVoices()
    {
      for(VoiceGroup &g : groups)
              g.freeFinishedVoices();
    }

    void allocSplitVoice(int key)
    {
      if(key < getSplitKey())
        allocSingleVoice (groupA);
      else
        allocSingleVoice (groupB);
    }

    void allocLayeredVoices()
    {
      allocSingleVoice (groupA);
      allocSingleVoice (groupB);
    }

    void allocSingleVoice(VoiceGroup group)
    {
      group.add(allocVoice());
    }

    Voice allocVoice()
    {
      return voicePool.removeFirst();
    }
};

