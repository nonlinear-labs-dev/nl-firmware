#pragma once

#include <memory>
#include <Types.h>
#include <synth/input/TCDDecoder.h>
#include <synth/input/MIDIDecoder.h>
#include <functional>
#include <nltools/messaging/Message.h>
#include <synth/c15-audio-engine/dsp_host_dual.h>

class MidiRuntimeOptions;

class InputEventStage
{
 public:
  using MIDIOutType = nltools::msg::Midi::SimpleMessage;
  using MIDIOut = std::function<void(MIDIOutType)>;

  InputEventStage(DSPInterface* dspHost, MidiRuntimeOptions* options, MIDIOut outCB);
  void onTCDMessage(const MidiEvent& tcdEvent);
  void onMIDIMessage(const MidiEvent& midiEvent);

  void setNoteShift(int i);

 private:
  TCDDecoder m_tcdDecoder;
  MIDIDecoder m_midiDecoder;
  DSPInterface* m_dspHost;
  MidiRuntimeOptions* m_options;
  MIDIOut m_midiOut;
  KeyShift m_shifteable_keys;

  bool checkMIDIKeyDownEnabled(MIDIDecoder* pDecoder);
  bool checkMIDIKeyUpEnabled(MIDIDecoder* pDecoder);
  bool checkMIDIHardwareChangeEnabled(MIDIDecoder* pDecoder);
  void onMIDIEvent(MIDIDecoder* decoder);

  void onTCDEvent(TCDDecoder* decoder);
  void sendKeyDownAsMidi(TCDDecoder* pDecoder);
  void convertToAndSendMIDI(TCDDecoder* pDecoder);
  void sendKeyUpAsMidi(TCDDecoder* pDecoder);
  void sendHardwareChangeAsMidi(TCDDecoder* pDecoder);
  void sendCCOut(int hwID, float value, int msbCC, int lsbCC);
  void doSendCCOut(uint16_t value, int msbCC, int lsbCC);
  VoiceGroup calculatePartForEvent(MIDIDecoder* pDecoder);
};