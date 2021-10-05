#include <synth/C15Synth.h>
#include "MidiRuntimeOptions.h"

void MidiRuntimeOptions::update(const tMidiSettingMessage& msg)
{
  m_midiPrimaryReceiveChannel = msg.receiveChannel;
  m_midiSplitReceiveChannel = msg.receiveSplitChannel;

  m_midiPrimarySendChannel = msg.sendChannel;
  m_midiSplitSendChannel = msg.sendSplitChannel;

  pedal1CC = msg.pedal1cc;
  pedal2CC = msg.pedal2cc;
  pedal3CC = msg.pedal3cc;
  pedal4CC = msg.pedal4cc;
  ribbon1CC = msg.ribbon1cc;
  ribbon2CC = msg.ribbon2cc;
  aftertouchCC = msg.aftertouchcc;
  benderCC = msg.bendercc;

  m_enableHighVelCC = msg.highVeloCCEnabled;
  m_enable14BitCC = msg.highResCCEnabled;

  m_routingMappings = msg.routings;
  m_globalLocalEnable = msg.globalLocalEnable;
}

MidiReceiveChannel MidiRuntimeOptions::getMIDIPrimaryReceiveChannel() const
{
  return m_midiPrimaryReceiveChannel;
}

MidiReceiveChannelSplit MidiRuntimeOptions::getMIDISplitReceiveChannel() const
{
  return m_midiSplitReceiveChannel;
}

MidiSendChannel MidiRuntimeOptions::getMIDIPrimarySendChannel() const
{
  return m_midiPrimarySendChannel;
}

MidiSendChannelSplit MidiRuntimeOptions::getMIDISplitSendChannel() const
{
  return m_midiSplitSendChannel;
}

bool MidiRuntimeOptions::shouldReceiveMIDIProgramChangesOnPrimary() const
{
  constexpr auto idx = static_cast<size_t>(tMidiSettingMessage::RoutingIndex::ProgramChange);
  constexpr auto aspect = static_cast<size_t>(tMidiSettingMessage::RoutingAspect::RECEIVE_PRIMARY);
  return m_routingMappings[idx][aspect];
}

bool MidiRuntimeOptions::shouldReceiveMIDIProgramChangesOnSplit() const
{
  constexpr auto idx = static_cast<size_t>(tMidiSettingMessage::RoutingIndex::ProgramChange);
  constexpr auto aspect = static_cast<size_t>(tMidiSettingMessage::RoutingAspect::RECEIVE_SPLIT);
  return m_routingMappings[idx][aspect];
}

bool MidiRuntimeOptions::shouldReceiveMIDINotesOnPrimary() const
{
  constexpr auto idx = static_cast<size_t>(tMidiSettingMessage::RoutingIndex::Notes);
  constexpr auto aspect = static_cast<size_t>(tMidiSettingMessage::RoutingAspect::RECEIVE_PRIMARY);
  return m_routingMappings[idx][aspect];
}

bool MidiRuntimeOptions::shouldReceiveMIDINotesOnSplit() const
{
  constexpr auto idx = static_cast<size_t>(tMidiSettingMessage::RoutingIndex::Notes);
  constexpr auto aspect = static_cast<size_t>(tMidiSettingMessage::RoutingAspect::RECEIVE_SPLIT);
  return m_routingMappings[idx][aspect];
}

bool MidiRuntimeOptions::shouldSendMIDIProgramChangesOnPrimary() const
{
  constexpr auto idx = static_cast<size_t>(tMidiSettingMessage::RoutingIndex::ProgramChange);
  constexpr auto aspect = static_cast<size_t>(tMidiSettingMessage::RoutingAspect::SEND_PRIMARY);
  return m_routingMappings[idx][aspect];
}

bool MidiRuntimeOptions::shouldSendMIDIProgramChangesOnSplit() const
{
  constexpr auto idx = static_cast<size_t>(tMidiSettingMessage::RoutingIndex::ProgramChange);
  constexpr auto aspect = static_cast<size_t>(tMidiSettingMessage::RoutingAspect::SEND_SPLIT);
  return m_routingMappings[idx][aspect];
}

bool MidiRuntimeOptions::shouldSendMIDINotesOnPrimary() const
{
  constexpr auto idx = static_cast<size_t>(tMidiSettingMessage::RoutingIndex::Notes);
  constexpr auto aspect = static_cast<size_t>(tMidiSettingMessage::RoutingAspect::SEND_PRIMARY);
  return m_routingMappings[idx][aspect];
}

bool MidiRuntimeOptions::shouldSendMIDINotesOnSplit() const
{
  constexpr auto idx = static_cast<size_t>(tMidiSettingMessage::RoutingIndex::Notes);
  constexpr auto aspect = static_cast<size_t>(tMidiSettingMessage::RoutingAspect::SEND_SPLIT);
  return m_routingMappings[idx][aspect];
}

bool MidiRuntimeOptions::shouldReceiveLocalNotes() const
{
  constexpr auto idx = static_cast<size_t>(MidiRuntimeOptions::tMidiSettingMessage::RoutingIndex::Notes);
  constexpr auto aspect = static_cast<size_t>(MidiRuntimeOptions::tMidiSettingMessage::RoutingAspect::LOCAL);
  return m_routingMappings[idx][aspect] && m_globalLocalEnable;
}

std::optional<int> MidiRuntimeOptions::decodeEnumMSB(PedalCC cc)
{
  switch(cc)
  {
    case PedalCC::CC01:
    case PedalCC::CC02:
    case PedalCC::CC03:
    case PedalCC::CC04:
    case PedalCC::CC05:
    case PedalCC::CC06:
    case PedalCC::CC07:
    case PedalCC::CC08:
    case PedalCC::CC09:
    case PedalCC::CC10:
    case PedalCC::CC11:
    case PedalCC::CC12:
    case PedalCC::CC13:
    case PedalCC::CC14:
    case PedalCC::CC15:
    case PedalCC::CC16:
    case PedalCC::CC17:
    case PedalCC::CC18:
    case PedalCC::CC19:
    case PedalCC::CC20:
    case PedalCC::CC21:
    case PedalCC::CC22:
    case PedalCC::CC23:
    case PedalCC::CC24:
    case PedalCC::CC25:
    case PedalCC::CC26:
    case PedalCC::CC27:
    case PedalCC::CC28:
    case PedalCC::CC29:
    case PedalCC::CC30:
    case PedalCC::CC31:
      return static_cast<int>(cc);
    case PedalCC::CC64:
    case PedalCC::CC65:
    case PedalCC::CC66:
    case PedalCC::CC67:
    case PedalCC::CC68:
    case PedalCC::CC69:
      return static_cast<int>(cc) + 32;
    default:
    case PedalCC::None:
      return std::nullopt;
  }
}

std::optional<int> MidiRuntimeOptions::decodeEnumLSB(PedalCC cc)
{
  switch(cc)
  {
    case PedalCC::CC01:
    case PedalCC::CC02:
    case PedalCC::CC03:
    case PedalCC::CC04:
    case PedalCC::CC05:
    case PedalCC::CC06:
    case PedalCC::CC07:
    case PedalCC::CC08:
    case PedalCC::CC09:
    case PedalCC::CC10:
    case PedalCC::CC11:
    case PedalCC::CC12:
    case PedalCC::CC13:
    case PedalCC::CC14:
    case PedalCC::CC15:
    case PedalCC::CC16:
    case PedalCC::CC17:
    case PedalCC::CC18:
    case PedalCC::CC19:
    case PedalCC::CC20:
    case PedalCC::CC21:
    case PedalCC::CC22:
    case PedalCC::CC23:
    case PedalCC::CC24:
    case PedalCC::CC25:
    case PedalCC::CC26:
    case PedalCC::CC27:
    case PedalCC::CC28:
    case PedalCC::CC29:
    case PedalCC::CC30:
    case PedalCC::CC31:
      return static_cast<int>(cc) + 32;
    case PedalCC::CC64:
    case PedalCC::CC65:
    case PedalCC::CC66:
    case PedalCC::CC67:
    case PedalCC::CC68:
    case PedalCC::CC69:
    default:
      return std::nullopt;
  }
}

std::optional<int> MidiRuntimeOptions::decodeEnumMSB(RibbonCC cc)
{
  if(cc == RibbonCC::None)
    return std::nullopt;
  return static_cast<int>(cc);
}

std::optional<int> MidiRuntimeOptions::decodeEnumLSB(RibbonCC cc)
{
  if(cc == RibbonCC::None)
    return std::nullopt;
  return static_cast<int>(cc) + 32;
}

std::optional<int> MidiRuntimeOptions::decodeEnumMSB(AftertouchCC cc)
{
  using ACC = AftertouchCC;

  if(cc == ACC::PitchbendUp || cc == ACC::PitchbendDown || cc == ACC::ChannelPressure || cc == ACC::None)
    return std::nullopt;
  else
    return static_cast<int>(cc) - 1;
}

std::optional<int> MidiRuntimeOptions::decodeEnumLSB(AftertouchCC cc)
{
  using ACC = AftertouchCC;
  if(cc == ACC::PitchbendUp || cc == ACC::PitchbendDown || cc == ACC::ChannelPressure || cc == ACC::None)
    return std::nullopt;
  else
    return static_cast<int>(cc) + 31;
}

std::optional<int> MidiRuntimeOptions::decodeEnumMSB(BenderCC cc)
{
  if(cc == BenderCC::Pitchbend || cc == BenderCC::None)
    return std::nullopt;
  else
    return static_cast<int>(cc) - 1;
}

std::optional<int> MidiRuntimeOptions::decodeEnumLSB(BenderCC cc)
{
  if(cc == BenderCC::Pitchbend || cc == BenderCC::None)
    return std::nullopt;
  else
    return static_cast<int>(cc) + 31;
}

int MidiRuntimeOptions::channelEnumToInt(MidiSendChannel channel)
{
  if(channel == MidiSendChannel::None)
    return -1;
  else
    return static_cast<int>(channel) - 1;
}

int MidiRuntimeOptions::channelEnumToInt(MidiSendChannelSplit channel)
{
  if(channel == MidiSendChannelSplit::None || channel == MidiSendChannelSplit::Common)
    return -1;
  //Maybe return Common as -2 if we have to differentiate them

  return static_cast<int>(channel) - 1;
}

int MidiRuntimeOptions::channelEnumToInt(MidiReceiveChannel channel)
{
  if(channel == MidiReceiveChannel::None)
    return -1;
  if(channel == MidiReceiveChannel::Omni)
    return -2;  //todo reevaluate

  return static_cast<int>(channel) - 2;
}

int MidiRuntimeOptions::channelEnumToInt(MidiReceiveChannelSplit channel)
{
  if(channel == MidiReceiveChannelSplit::None || channel == MidiReceiveChannelSplit::Common)
    return -1;
  if(channel == MidiReceiveChannelSplit::Omni)
    return -2;  //todo reevaluate

  return static_cast<int>(channel) - 2;
}

void MidiRuntimeOptions::setBenderCC(BenderCC cc)
{
  benderCC = cc;
}

void MidiRuntimeOptions::setAftertouchCC(AftertouchCC cc)
{
  aftertouchCC = cc;
}

std::optional<int> MidiRuntimeOptions::getBenderMSBCC() const
{
  return decodeEnumMSB(benderCC);
}

std::optional<int> MidiRuntimeOptions::getBenderLSBCC() const
{
  return decodeEnumLSB(benderCC);
}

void MidiRuntimeOptions::setPedal1(PedalCC cc)
{
  pedal1CC = cc;
}

void MidiRuntimeOptions::setSendSplitChannel(MidiSendChannelSplit c)
{
  m_midiSplitSendChannel = c;
}

void MidiRuntimeOptions::setSendChannel(MidiSendChannel c)
{
  m_midiPrimarySendChannel = c;
}

void MidiRuntimeOptions::setReceiveChannel(MidiReceiveChannel c)
{
  m_midiPrimaryReceiveChannel = c;
}
void MidiRuntimeOptions::setSplitReceiveChannel(MidiReceiveChannelSplit c)
{
  m_midiSplitReceiveChannel = c;
}

MidiReceiveChannelSplit MidiRuntimeOptions::normalToSplitChannel(MidiReceiveChannel ch)
{
  return static_cast<MidiReceiveChannelSplit>(ch);
}

std::optional<int> MidiRuntimeOptions::getAftertouchLSBCC() const
{
  return decodeEnumLSB(aftertouchCC);
}

std::optional<int> MidiRuntimeOptions::getAftertouchMSBCC() const
{
  return decodeEnumMSB(aftertouchCC);
}

AftertouchCC MidiRuntimeOptions::getAftertouchSetting() const
{
  return aftertouchCC;
}

bool MidiRuntimeOptions::isSwitchingCC(int pedalZeroIndexed) const
{
  auto enumIsInSwitching = [](PedalCC cc) -> bool {
    switch(cc)
    {
      case PedalCC::CC64:
      case PedalCC::CC65:
      case PedalCC::CC66:
      case PedalCC::CC67:
      case PedalCC::CC68:
      case PedalCC::CC69:
        return true;
      default:
        return false;
    }
  };

  switch(pedalZeroIndexed)
  {
    case 0:
      return enumIsInSwitching(pedal1CC);
    case 1:
      return enumIsInSwitching(pedal2CC);
    case 2:
      return enumIsInSwitching(pedal3CC);
    case 3:
      return enumIsInSwitching(pedal4CC);
    default:
      return false;
  }
}

bool MidiRuntimeOptions::enableHighVelCC() const
{
  return m_enableHighVelCC;
}

int MidiRuntimeOptions::getMSBCCForHWID(int hwID) const
{
  auto bender = getBenderMSBCC();
  auto aftertouch = getAftertouchMSBCC();

  switch(hwID)
  {
    case 0:
      return getCCFor<Midi::MSB::Ped1>();
    case 1:
      return getCCFor<Midi::MSB::Ped2>();
    case 2:
      return getCCFor<Midi::MSB::Ped3>();
    case 3:
      return getCCFor<Midi::MSB::Ped4>();
    case 4:
      return bender.value_or(-1);
    case 5:
      return aftertouch.value_or(-1);
    case 6:
      return getCCFor<Midi::MSB::Rib1>();
    case 7:
      return getCCFor<Midi::MSB::Rib2>();
    default:
      return -1;
  }
}

BenderCC MidiRuntimeOptions::getBenderSetting() const
{
  return benderCC;
}

void MidiRuntimeOptions::setPedal2(PedalCC cc)
{
  pedal2CC = cc;
}

void MidiRuntimeOptions::setPedal3(PedalCC cc)
{
  pedal3CC = cc;
}

void MidiRuntimeOptions::setPedal4(PedalCC cc)
{
  pedal4CC = cc;
}

void MidiRuntimeOptions::setRibbon1(RibbonCC cc)
{
  ribbon1CC = cc;
}

void MidiRuntimeOptions::setRibbon2(RibbonCC cc)
{
  ribbon2CC = cc;
}

bool MidiRuntimeOptions::is14BitSupportEnabled() const
{
  return m_enable14BitCC;
}

void MidiRuntimeOptions::set14BitSupportEnabled(bool e)
{
  m_enable14BitCC = e;
}

bool MidiRuntimeOptions::shouldReceiveMidiOnPrimary(tMidiSettingMessage::RoutingIndex routingIndex) const
{
  constexpr auto receivePrim = static_cast<int>(tRoutingAspect::RECEIVE_PRIMARY);
  const auto index = static_cast<int>(routingIndex);
  return m_routingMappings[index][receivePrim];
}

bool MidiRuntimeOptions::shouldSendMidiOnPrimary(tMidiSettingMessage::RoutingIndex routingIndex) const
{
  constexpr auto sendPrim = static_cast<int>(tRoutingAspect::SEND_PRIMARY);
  const auto index = static_cast<int>(routingIndex);
  return m_routingMappings[index][sendPrim];
}

bool MidiRuntimeOptions::shouldReceiveMidiOnSplit(tMidiSettingMessage::RoutingIndex routingIndex) const
{
  constexpr auto receiveSplit = static_cast<int>(tRoutingAspect::RECEIVE_SPLIT);
  const auto index = static_cast<int>(routingIndex);
  return m_routingMappings[index][receiveSplit];
}

bool MidiRuntimeOptions::shouldSendMidiOnSplit(tMidiSettingMessage::RoutingIndex routingIndex) const
{
  constexpr auto sendSplit = static_cast<int>(tRoutingAspect::SEND_SPLIT);
  const auto index = static_cast<int>(routingIndex);
  return m_routingMappings[index][sendSplit];
}

bool MidiRuntimeOptions::shouldAllowLocal(tMidiSettingMessage::RoutingIndex routingIndex) const
{
  constexpr auto local = static_cast<int>(tRoutingAspect::LOCAL);
  const auto index = static_cast<int>(routingIndex);
  return m_routingMappings[index][local] && m_globalLocalEnable;
}

bool MidiRuntimeOptions::isCCMappedToChannelModeMessage(int cc)
{
  switch(cc)
  {
    case static_cast<int>(MidiChannelModeMessageCCs::AllSoundOff):
    case static_cast<int>(MidiChannelModeMessageCCs::AllNotesOff):
    case static_cast<int>(MidiChannelModeMessageCCs::LocalControlOnOff):
      return true;
    default:
      break;
  }
  return false;
}

MidiChannelModeMessages MidiRuntimeOptions::createChannelModeMessageEnum(int cc, uint8_t ccValue)
{
  switch(cc)
  {
    case static_cast<int>(MidiChannelModeMessageCCs::AllSoundOff):
      if(ccValue == 0)
        return MidiChannelModeMessages::AllSoundOff;
      break;
    case static_cast<int>(MidiChannelModeMessageCCs::AllNotesOff):
      if(ccValue == 0)
        return MidiChannelModeMessages::AllNotesOff;
      break;
    case static_cast<int>(MidiChannelModeMessageCCs::ResetAllControllers):
      if(ccValue == 0)
        return MidiChannelModeMessages::ResetAllControllers;
      break;
    case static_cast<int>(MidiChannelModeMessageCCs::LocalControlOnOff):
      if(ccValue == 0)
        return MidiChannelModeMessages::LocalControllersOff;
      else if(ccValue == 127)
        return MidiChannelModeMessages::LocalControllersOn;
      break;
    default:
      return MidiChannelModeMessages::NOOP;
  }
  return MidiChannelModeMessages::NOOP;
}

bool MidiRuntimeOptions::isLocalEnabled(C15::Parameters::Hardware_Sources sources)
{
  switch(sources)
  {
    case C15::Parameters::Hardware_Sources::Pedal_1:
      return shouldAllowLocal(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Pedal1);
    case C15::Parameters::Hardware_Sources::Pedal_2:
      return shouldAllowLocal(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Pedal2);
    case C15::Parameters::Hardware_Sources::Pedal_3:
      return shouldAllowLocal(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Pedal3);
    case C15::Parameters::Hardware_Sources::Pedal_4:
      return shouldAllowLocal(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Pedal4);
    case C15::Parameters::Hardware_Sources::Bender:
      return shouldAllowLocal(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Bender);
    case C15::Parameters::Hardware_Sources::Aftertouch:
      return shouldAllowLocal(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Aftertouch);
    case C15::Parameters::Hardware_Sources::Ribbon_1:
      return shouldAllowLocal(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Ribbon1);
    case C15::Parameters::Hardware_Sources::Ribbon_2:
      return shouldAllowLocal(nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Ribbon2);
    case C15::Parameters::Hardware_Sources::_LENGTH_:
      return false;
  }
  return false;
}

void MidiRuntimeOptions::setGlobalLocalEnabled(bool b)
{
  m_globalLocalEnable = b;
}
