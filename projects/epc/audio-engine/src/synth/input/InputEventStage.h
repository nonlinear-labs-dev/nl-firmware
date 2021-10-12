#pragma once

#include <memory>
#include <Types.h>
#include <synth/input/TCDDecoder.h>
#include <synth/input/MIDIDecoder.h>
#include <functional>
#include <nltools/messaging/Message.h>
#include <synth/c15-audio-engine/dsp_host_dual.h>
#include "MidiChannelModeMessages.h"
#include <Types.h>

class MidiRuntimeOptions;

class InputEventStage
{
 public:
  using RoutingIndex = nltools::msg::Setting::MidiSettingsMessage::RoutingIndex;
  using RoutingAspect = nltools::msg::Setting::MidiSettingsMessage::RoutingAspect;
  using MIDIOutType = nltools::msg::Midi::SimpleMessage;
  using MIDIOut = std::function<void(MIDIOutType)>;
  using HWChangedNotification = std::function<void()>;
  using ChannelModeMessageCB = std::function<void(MidiChannelModeMessages)>;

  //use reference
  InputEventStage(DSPInterface* dspHost, MidiRuntimeOptions* options, HWChangedNotification hwChangedCB, MIDIOut outCB,
                  ChannelModeMessageCB specialFunctionOut);
  void onTCDMessage(const MidiEvent& tcdEvent);
  void onMIDIMessage(const MidiEvent& midiEvent);
  void onUIHWSourceMessage(const nltools::msg::HWSourceChangedMessage& message, bool didBehaviourChange);
  void setNoteShift(int i);
  [[nodiscard]] int getNoteShift() const;

  using tMSG = nltools::msg::Setting::MidiSettingsMessage;
  using tRow = tMSG::tEntry;

  [[nodiscard]] float getHWSourcePositionIfLocalDisabled(HardwareSource hwid) const;
  [[nodiscard]] HWChangeSource getHWSourcePositionSource(HardwareSource hwid) const;

  void onMidiSettingsMessageWasReceived(const tMSG& msg, const tMSG& oldmsg);

  static HardwareSource parameterIDToHWID(int id);
  bool getAndResetKeyBedStatus();

 private:
  void handlePressedNotesOnMidiSettingsChanged(const tMSG& msg, const tMSG& oldmsg);
  void handleHWSourcesWhichGotTurnedOff(const tMSG& msg, const tMSG& snapshot);
  void handleHWSourcesWhichCCsChanged(const tMSG& message, const tMSG& snapshot);

  void setAndScheduleKeybedNotify();

  using CC_Range_Vel = Midi::clipped14BitVelRange;
  using CC_Range_7_Bit = Midi::FullCCRange<Midi::Formats::_7_Bits_>;

  //TCD/MIDI In
  bool checkMIDIKeyEventEnabled(MIDIDecoder* pDecoder);
  bool checkMIDIHardwareChangeChannelMatches(MIDIDecoder* pDecoder);
  void onMIDIEvent();
  void onTCDEvent();
  void onMIDIHWChanged(MIDIDecoder* decoder);

  //Algorithm
  void onHWChanged(HardwareSource hwID, float pos, HWChangeSource source, bool wasMIDIPrimary, bool wasMIDISplit,
                   bool didBehaviourChange);

  VoiceGroup calculateSplitPartForKeyDown(DSPInterface::InputEventSource inputEvent, int keyNumber);
  VoiceGroup calculateSplitPartForKeyUp(DSPInterface::InputEventSource inputEvent, int keyNumber);
  DSPInterface::InputEventSource getInputSourceFromParsedChannel(MidiReceiveChannel channel);

  static constexpr uint16_t midiReceiveChannelMask(const MidiReceiveChannel& _channel);
  static constexpr uint16_t midiReceiveChannelMask(const MidiReceiveChannelSplit& _channel);

  //MIDI and UI out
  void convertToAndSendMIDI(TCDDecoder* pDecoder, const VoiceGroup& determinedPart);
  void sendKeyDownAsMidi(TCDDecoder* pDecoder, const VoiceGroup& determinedPart);
  void sendKeyUpAsMidi(TCDDecoder* pDecoder, const VoiceGroup& determinedPart);
  void sendHardwareChangeAsMidi(HardwareSource hwID, float value);
  void doSendAftertouchOut(float value);
  void doSendBenderOut(float value);

  void sendCCOut(HardwareSource hwID, float value, int msbCC, int lsbCC);
  void doSendCCOut(uint16_t value, int msbCC, int lsbCC, HardwareSource hwID);

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

  TCDDecoder m_tcdDecoder;
  MIDIDecoder m_midiDecoder;
  DSPInterface* m_dspHost;
  MidiRuntimeOptions* m_options;
  HWChangedNotification m_hwChangedCB;
  ChannelModeMessageCB m_channelModeMessageCB;

  MIDIOut m_midiOut;
  KeyShift m_shifteable_keys;
  constexpr static auto NUM_HW = 8;
  std::array<std::array<uint16_t, 2>, NUM_HW> m_latchedHWPositions {};

  using tHWPosEntry = std::tuple<float, HWChangeSource>;
  std::array<tHWPosEntry, NUM_HW> m_localDisabledPositions;

  bool m_notifyKeyBedActionStatus = false;

  enum class LatchMode
  {
    Option,
    LSBAndMSB,
    OnlyMSB
  };

  template <LatchMode> bool latchHWPosition(HardwareSource hwID, uint8_t lsb, uint8_t msb);
  [[nodiscard]] bool isSplitDSP() const;

  friend class InputEventStageTester;
  static bool ccIsMappedToChannelModeMessage(int cc);
  void queueChannelModeMessage(int cc, uint8_t msbCCvalue);

  void sendHardwareChangeAsMidiOnExplicitChannel(HardwareSource id, float value, int channel);
  void sendCCOutOnExplicitChannel(HardwareSource hwID, float value, int msbCC, int lsbCC, int channel);
  void doSendCCOutOnExplicitChannel(uint16_t value, int msbCC, int lsbCC, HardwareSource hwID, int channel);
  void doSendBenderOutOnExplicitChannel(float value, int channel);
  void doSendAftertouchOutOnExplicitChannel(float value, int channel);
  RoutingIndex toRoutingIndex(HardwareSource source);
};
