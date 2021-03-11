#pragma once
#include <nltools/messaging/Message.h>
#include <variant>

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
  static int channelEnumToInt(MidiReceiveChannel channel);

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

  static int decodeEnumMSB(PedalCC);
  static int decodeEnumLSB(PedalCC);

  static int decodeEnumMSB(RibbonCC);
  static int decodeEnumLSB(RibbonCC);

  static std::pair<bool, int> decodeEnumMSB(AftertouchCC);
  static std::pair<bool, int> decodeEnumLSB(AftertouchCC);

  static std::pair<bool, int> decodeEnumMSB(BenderCC);
  static std::pair<bool, int> decodeEnumLSB(BenderCC);
};
