#pragma once
#include <Application.h>
#include <synth/c15-audio-engine/dsp_host_dual.h>
#include <Types.h>

class InputEventStage;
class TCDDecoder;
class MIDIDecoder;
class DSPInterface;
class MidiRuntimeOptions;

class InputEventStageTester
{
 public:
  explicit InputEventStageTester(InputEventStage* stage);
  MidiRuntimeOptions* getOptions();
  void onTCDMessage(const MidiEvent& tcdEvent);
  void onMIDIMessage(const MidiEvent& midiEvent);
  DSPInterface::InputEventSource getInputSourceFromParsedChannel(MidiReceiveChannel channel);

 private:
  InputEventStage* m_inputStage;
};
