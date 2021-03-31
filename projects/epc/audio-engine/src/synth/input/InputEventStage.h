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

  bool checkMIDIKeyEventEnabled(MIDIDecoder* pDecoder);
  bool checkMIDIHardwareChangeEnabled(MIDIDecoder* pDecoder);
  void onMIDIEvent(MIDIDecoder* decoder);

  void onTCDEvent(TCDDecoder* decoder);
  void sendKeyDownAsMidi(TCDDecoder* pDecoder, const VoiceGroup& determinedPart);
  void convertToAndSendMIDI(TCDDecoder* pDecoder, const VoiceGroup& determinedPart);
  void sendKeyUpAsMidi(TCDDecoder* pDecoder, const VoiceGroup& determinedPart);
  void sendHardwareChangeAsMidi(TCDDecoder* pDecoder);
  void sendCCOut(int hwID, float value, int msbCC, int lsbCC);
  void doSendCCOut(uint16_t value, int msbCC, int lsbCC);
  VoiceGroup calculateSplitPartForEvent(DSPInterface::InputEvent inputEvent, const int keyNumber);
  DSPInterface::InputEvent getInterfaceFromParsedChannel(MidiReceiveChannel channel);

  friend class InputEventStageTester;

  static constexpr uint16_t c_midiReceiveMaskTable[19] = {
    0x0000,  // None (no bit is set)
    0xFFFF,  // Omni (all bits are set)
    0x0001,  // CH_1 (specific channels 0..15 refer to specific bits 0..15)
    0x0002,  // CH_2
    0x0004,  // CH_3
    0x0008,  // CH_4
    0x0010,  // CH_5
    0x0020,  // CH_6
    0x0040,  // CH_7
    0x0080,  // CH_8
    0x0100,  // CH_9
    0x0200,  // CH_10
    0x0400,  // CH_11
    0x0800,  // CH_12
    0x1000,  // CH_13
    0x2000,  // CH_14
    0x4000,  // CH_15
    0x8000,  // CH_16
    0x0000   // Common = None
  };

  static constexpr uint16_t midiReceiveChannelMask(const MidiReceiveChannel& _channel)
  {
    return c_midiReceiveMaskTable[static_cast<uint8_t>(_channel)];
  }

  static constexpr uint16_t midiReceiveChannelMask(const MidiReceiveChannelSplit& _channel)
  {
    return c_midiReceiveMaskTable[static_cast<uint8_t>(_channel)];
  }

  static constexpr uint16_t midiReceiveChannelMask(const uint8_t& _channel)
  {
    if(_channel > 15)
      return 0;                                   // fail-safe (but a midi channel shouldn't go beyond 15 anyway)
    return c_midiReceiveMaskTable[2 + _channel];  // we read out the mask matching the specific channel id
  }
  void doSendAftertouchOut(uint8_t cc, uint8_t value);
  void doSendBenderOut(uint8_t lsb, uint8_t msb);
};

namespace InputStateDetail
{
  using Event = DSPInterface::InputEvent;
  using State = DSPInterface::InputState;
  using Source = DSPInterface::InputSource;

  static constexpr Event Unknown = { Source::Unknown, State::Invalid };
  static constexpr Event TCD = { Source::TCD, State::Singular };
  static constexpr Event Singular = { Source::Primary, State::Singular };
  static constexpr Event Primary = { Source::Primary, State::Separate };
  static constexpr Event Both = { Source::Both, State::Separate };
  static constexpr Event Secondary = { Source::Secondary, State::Separate };
}
