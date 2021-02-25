#pragma once

#include <memory>
#include <Types.h>
#include <synth/input/TCDDecoder.h>
#include <synth/input/MIDIDecoder.h>
#include <functional>

class dsp_host_dual;
class MidiRuntimeOptions;

class InputEventStage
{
 public:
  using MIDIOutType = nltools::msg::Midi::SimpleMessage;
  using MIDIOut = std::function<void(MIDIOutType)>;
  InputEventStage(dsp_host_dual* dspHost, MidiRuntimeOptions* options, MIDIOut outCB);
  void onTCDMessage(const MidiEvent& tcdEvent);
  void onMIDIMessage(const MidiEvent& midiEvent);

 private:
  std::unique_ptr<TCDDecoder> m_tcdDecoder;
  std::unique_ptr<MIDIDecoder> m_midiDecoder;
  dsp_host_dual* m_dspHost;
  MidiRuntimeOptions* m_options;
  MIDIOut m_midiOut;

  void onTCDEvent(TCDDecoder* decoder);
  void onMIDIEvent(MIDIDecoder* decoder);
  void convertToAndSendMIDI(TCDDecoder* pDecoder);
};