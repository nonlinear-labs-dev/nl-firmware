#pragma once
#include <nltools/messaging/Message.h>
#include <variant>
#include <synth/c15-audio-engine/midi_handle.h>

class MidiRuntimeOptions
{
 public:
  [[nodiscard]] MidiReceiveChannel getReceiveChannel() const;
  [[nodiscard]] MidiReceiveChannelSplit getReceiveSplitChannel() const;
  [[nodiscard]] MidiSendChannel getSendChannel() const;
  [[nodiscard]] MidiSendChannelSplit getSendSplitChannel() const;

  [[nodiscard]] bool shouldReceiveProgramChanges() const;
  [[nodiscard]] bool shouldReceiveNotes() const;
  [[nodiscard]] bool shouldReceiveControllers() const;

  [[nodiscard]] bool shouldSendProgramChanges() const;
  [[nodiscard]] bool shouldSendNotes() const;
  [[nodiscard]] bool shouldSendControllers() const;

  [[nodiscard]] bool shouldReceiveLocalNotes() const;
  [[nodiscard]] bool shouldReceiveLocalControllers() const;

  void update(const nltools::msg::Setting::MidiSettingsMessage& msg);

  int ccToMSBHardwareControlID(uint8_t i);
  int ccToLSBHardwareControlID(uint8_t i);

  static int channelEnumToInt(MidiSendChannel channel);
  int channelEnumToInt(MidiSendChannelSplit channel);
  static int channelEnumToInt(MidiReceiveChannel channel);
  static MidiReceiveChannelSplit normalToSplitChannel(MidiReceiveChannel ch);

  void setBenderCC(BenderCC cc);
  void setAftertouchCC(AftertouchCC cc);
  void setSendSplitChannel(MidiSendChannelSplit c);
  void setSendChannel(MidiSendChannel c);
  void setReceiveChannel(MidiReceiveChannel c);
  void setSplitReceiveChannel(MidiReceiveChannelSplit c);

  template <Midi::LSB::HWSourceMidiCC tLSB> int getCCFor()
  {
    if constexpr(tLSB == Midi::LSB::Ped1)
      return decodeEnumLSB(pedal1CC);
    else if constexpr(tLSB == Midi::LSB::Ped2)
      return decodeEnumLSB(pedal2CC);
    else if constexpr(tLSB == Midi::LSB::Ped3)
      return decodeEnumLSB(pedal3CC);
    else if constexpr(tLSB == Midi::LSB::Ped4)
      return decodeEnumLSB(pedal4CC);
    else if constexpr(tLSB == Midi::LSB::Rib1)
      return decodeEnumLSB(ribbon1CC);
    else if constexpr(tLSB == Midi::LSB::Rib2)
      return decodeEnumLSB(ribbon2CC);
    else
      nltools_assertNotReached();
  }

  template <Midi::MSB::HWSourceMidiCC tMSB> int getCCFor()
  {
    if constexpr(tMSB == Midi::MSB::Ped1)
      return decodeEnumMSB(pedal1CC);
    else if constexpr(tMSB == Midi::MSB::Ped2)
      return decodeEnumMSB(pedal2CC);
    else if constexpr(tMSB == Midi::MSB::Ped3)
      return decodeEnumMSB(pedal3CC);
    else if constexpr(tMSB == Midi::MSB::Ped4)
      return decodeEnumMSB(pedal4CC);
    else if constexpr(tMSB == Midi::MSB::Rib1)
      return decodeEnumMSB(ribbon1CC);
    else if constexpr(tMSB == Midi::MSB::Rib2)
      return decodeEnumMSB(ribbon2CC);
    else
      nltools_assertNotReached();
  }

  std::pair<bool, int> getBenderMSBCC();
  std::pair<bool, int> getBenderLSBCC();
  std::pair<bool, int> getAftertouchMSBCC();
  std::pair<bool, int> getAftertouchLSBCC();
  AftertouchCC getAftertouchSetting() const;

  void setPedal1(PedalCC cc);

  bool isSwitchingCC(int pedalZeroIndexed);

 private:
  MidiReceiveChannel m_receiveChannel;
  MidiReceiveChannelSplit m_receiveSplitChannel;
  MidiSendChannel m_sendChannel;
  MidiSendChannelSplit m_sendSplitChannel;

  bool m_receiveProgramChanges = false;
  bool m_receiveNotes = false;
  bool m_receiveControllers = false;

  bool m_sendProgramChanges = false;
  bool m_sendNotes = false;
  bool m_sendControllers = false;

  bool m_localNotes = false;
  bool m_localControllers = false;

  PedalCC pedal1CC;
  PedalCC pedal2CC;
  PedalCC pedal3CC;
  PedalCC pedal4CC;
  RibbonCC ribbon1CC;
  RibbonCC ribbon2CC;
  AftertouchCC aftertouchCC;
  BenderCC benderCC;

  //Mappings
  static int decodeEnumMSB(PedalCC);
  static int decodeEnumLSB(PedalCC);

  static int decodeEnumMSB(RibbonCC);
  static int decodeEnumLSB(RibbonCC);

  static std::pair<bool, int> decodeEnumMSB(AftertouchCC);
  static std::pair<bool, int> decodeEnumLSB(AftertouchCC);

  static std::pair<bool, int> decodeEnumMSB(BenderCC);
  static std::pair<bool, int> decodeEnumLSB(BenderCC);
};
