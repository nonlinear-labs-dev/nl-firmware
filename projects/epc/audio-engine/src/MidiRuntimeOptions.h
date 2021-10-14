#pragma once
#include <nltools/messaging/Message.h>
#include <variant>
#include <synth/c15-audio-engine/midi_handle.h>
#include <synth/input/MidiChannelModeMessages.h>
#include <Types.h>

class MidiRuntimeOptions
{
 public:
  typedef nltools::msg::Setting::MidiSettingsMessage tMidiSettingMessage;
  typedef tMidiSettingMessage::tEntry tRoutingEntry;
  typedef tMidiSettingMessage::RoutingAspect tRoutingAspect;
  typedef tMidiSettingMessage::RoutingIndex tRoutingIndex;

  enum class MidiChannelModeMessageCCs : int {
    AllSoundOff = 120,
    ResetAllControllers = 121,
    LocalControlOnOff = 122,
    AllNotesOff = 123
  };

  [[nodiscard]] MidiReceiveChannel getMIDIPrimaryReceiveChannel() const;
  [[nodiscard]] MidiReceiveChannelSplit getMIDISplitReceiveChannel() const;
  [[nodiscard]] MidiSendChannel getMIDIPrimarySendChannel() const;
  [[nodiscard]] MidiSendChannelSplit getMIDISplitSendChannel() const;
  [[nodiscard]] AftertouchCC getAftertouchSetting() const;
  [[nodiscard]] BenderCC getBenderSetting() const;

  [[nodiscard]] bool shouldReceiveMIDIProgramChangesOnPrimary() const;
  [[nodiscard]] bool shouldReceiveMIDIProgramChangesOnSplit() const;
  [[nodiscard]] bool shouldReceiveMIDINotesOnPrimary() const;
  [[nodiscard]] bool shouldReceiveMIDINotesOnSplit() const;

  [[nodiscard]] bool shouldSendMIDIProgramChangesOnPrimary() const;
  [[nodiscard]] bool shouldSendMIDIProgramChangesOnSplit() const;
  [[nodiscard]] bool shouldSendMIDINotesOnPrimary() const;
  [[nodiscard]] bool shouldSendMIDINotesOnSplit() const;

  [[nodiscard]] bool shouldReceiveLocalNotes() const;

  [[nodiscard]] bool shouldReceiveMidiOnPrimary(tMidiSettingMessage::RoutingIndex routingIndex) const;
  [[nodiscard]] bool shouldReceiveMidiOnSplit(tMidiSettingMessage::RoutingIndex routingIndex) const;
  [[nodiscard]] bool shouldSendMidiOnPrimary(tMidiSettingMessage::RoutingIndex routingIndex) const;
  [[nodiscard]] bool shouldSendMidiOnSplit(tMidiSettingMessage::RoutingIndex routingIndex) const;
  [[nodiscard]] bool shouldAllowLocal(tMidiSettingMessage::RoutingIndex routingIndex) const;

  const nltools::msg::Setting::MidiSettingsMessage& getLastReceivedMessage() const;
  void update(const nltools::msg::Setting::MidiSettingsMessage& msg);

  static int channelEnumToInt(MidiSendChannel channel);
  static int channelEnumToInt(MidiSendChannelSplit channel);
  static int channelEnumToInt(MidiReceiveChannel channel);
  static int channelEnumToInt(MidiReceiveChannelSplit channel);
  static MidiReceiveChannelSplit normalToSplitChannel(MidiReceiveChannel ch);

  template <Midi::LSB::HWSourceMidiCC tLSB> [[nodiscard]] int getCCFor() const
  {
    if constexpr(tLSB == Midi::LSB::Ped1)
      return decodeEnumLSB(pedal1CC).value_or(-1);
    else if constexpr(tLSB == Midi::LSB::Ped2)
      return decodeEnumLSB(pedal2CC).value_or(-1);
    else if constexpr(tLSB == Midi::LSB::Ped3)
      return decodeEnumLSB(pedal3CC).value_or(-1);
    else if constexpr(tLSB == Midi::LSB::Ped4)
      return decodeEnumLSB(pedal4CC).value_or(-1);
    else if constexpr(tLSB == Midi::LSB::Rib1)
      return decodeEnumLSB(ribbon1CC).value_or(-1);
    else if constexpr(tLSB == Midi::LSB::Rib2)
      return decodeEnumLSB(ribbon2CC).value_or(-1);
    else
      nltools_assertNotReached();
  }
  template <Midi::MSB::HWSourceMidiCC tMSB> [[nodiscard]] int getCCFor() const
  {
    if constexpr(tMSB == Midi::MSB::Ped1)
      return decodeEnumMSB(pedal1CC).value_or(-1);
    else if constexpr(tMSB == Midi::MSB::Ped2)
      return decodeEnumMSB(pedal2CC).value_or(-1);
    else if constexpr(tMSB == Midi::MSB::Ped3)
      return decodeEnumMSB(pedal3CC).value_or(-1);
    else if constexpr(tMSB == Midi::MSB::Ped4)
      return decodeEnumMSB(pedal4CC).value_or(-1);
    else if constexpr(tMSB == Midi::MSB::Rib1)
      return decodeEnumMSB(ribbon1CC).value_or(-1);
    else if constexpr(tMSB == Midi::MSB::Rib2)
      return decodeEnumMSB(ribbon2CC).value_or(-1);
    else if constexpr(tMSB == Midi::MSB::Aftertouch)
      return decodeEnumMSB(aftertouchCC).value_or(-1);
    else if constexpr(tMSB == Midi::MSB::Bender)
      return decodeEnumMSB(benderCC).value_or(-1);
    else
      nltools_assertNotReached();
  }

  [[nodiscard]] std::optional<int> getBenderMSBCC() const;
  [[nodiscard]] std::optional<int> getBenderLSBCC() const;
  [[nodiscard]] std::optional<int> getAftertouchMSBCC() const;
  [[nodiscard]] std::optional<int> getAftertouchLSBCC() const;


  [[nodiscard]] bool isSwitchingCC(HardwareSource hwid) const;
  [[nodiscard]] bool enableHighVelCC() const;
  [[nodiscard]] bool is14BitSupportEnabled() const;
  [[nodiscard]] int getMSBCCForHWID(HardwareSource hwID) const;

  static bool isCCMappedToChannelModeMessage(int cc);
  static MidiChannelModeMessages createChannelModeMessageEnum(int cc, uint8_t ccValue);

  void setPedal1(PedalCC cc);
  void setPedal2(PedalCC cc);
  void setPedal3(PedalCC cc);
  void setPedal4(PedalCC cc);
  void setRibbon1(RibbonCC cc);
  void setRibbon2(RibbonCC cc);
  void setBenderCC(BenderCC cc);
  void setAftertouchCC(AftertouchCC cc);
  void setSendSplitChannel(MidiSendChannelSplit c);
  void setSendChannel(MidiSendChannel c);
  void setReceiveChannel(MidiReceiveChannel c);
  void setSplitReceiveChannel(MidiReceiveChannelSplit c);
  void set14BitSupportEnabled(bool e);

  bool isMidiSafeModeEnabled();

  bool isGlobalLocalEnabled();
  void setGlobalLocalEnabled(bool b);
  bool isLocalEnabled(HardwareSource source);

  //Mappings
  static std::optional<int> decodeEnumMSB(PedalCC);
  static std::optional<int> decodeEnumLSB(PedalCC);
  static std::optional<int> decodeEnumMSB(RibbonCC);
  static std::optional<int> decodeEnumLSB(RibbonCC);

  static std::optional<int> decodeEnumMSB(AftertouchCC cc);
  static std::optional<int> decodeEnumLSB(AftertouchCC cc);
  static std::optional<int> decodeEnumMSB(BenderCC cc);
  static std::optional<int> decodeEnumLSB(BenderCC cc);

 private:
  MidiReceiveChannel m_midiPrimaryReceiveChannel;
  MidiReceiveChannelSplit m_midiSplitReceiveChannel;
  MidiSendChannel m_midiPrimarySendChannel;
  MidiSendChannelSplit m_midiSplitSendChannel;

  bool m_safeMode = true;
  bool m_localEnable = true;

  bool m_enableHighVelCC = false;
  bool m_enable14BitCC = false;

  PedalCC pedal1CC;
  PedalCC pedal2CC;
  PedalCC pedal3CC;
  PedalCC pedal4CC;
  RibbonCC ribbon1CC;
  RibbonCC ribbon2CC;
  AftertouchCC aftertouchCC;
  BenderCC benderCC;

  nltools::msg::Setting::MidiSettingsMessage::tRoutingMappings m_routingMappings;

  nltools::msg::Setting::MidiSettingsMessage m_lastMessage;

  friend class MidiRuntimeOptionsTester;
};
