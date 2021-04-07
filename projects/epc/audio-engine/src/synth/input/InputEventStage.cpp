#include <MidiRuntimeOptions.h>

#include <utility>
#include "synth/c15-audio-engine/dsp_host_dual.h"
#include "InputEventStage.h"

InputEventStage::InputEventStage(DSPInterface *dspHost, MidiRuntimeOptions *options, InputEventStage::MIDIOut outCB)
    : m_dspHost { dspHost }
    , m_options { options }
    , m_midiOut { std::move(outCB) }
    , m_midiDecoder(dspHost, options)
    , m_tcdDecoder(dspHost, options, &m_shifteable_keys)
{
}

void InputEventStage::onTCDMessage(const MidiEvent &tcdEvent)
{
  if(m_tcdDecoder.decode(tcdEvent))
  {
    onTCDEvent(&m_tcdDecoder);
    m_tcdDecoder.reset();
  }
}

void InputEventStage::onMIDIMessage(const MidiEvent &midiEvent)
{
  if(m_midiDecoder.decode(midiEvent))
  {
    onMIDIEvent(&m_midiDecoder);
    m_midiDecoder.reset();
  }
}

void InputEventStage::onTCDEvent(TCDDecoder *decoder)
{
  if(decoder)
  {
    VoiceGroup determinedPart = VoiceGroup::Global;    // initially, we set it to global
    const SoundType soundType = m_dspHost->getType();  // also, we track the sound type for more safety
    const bool soundValid = soundType != SoundType::Invalid;
    const auto interface = InputStateDetail::TCD;

    switch(decoder->getEventType())
    {
      case DecoderEventType::KeyDown:
        if(m_options->shouldReceiveLocalNotes())
        {
          if(soundType == SoundType::Split)
          {
            determinedPart = calculateSplitPartForEvent(interface, decoder->getKeyOrController());
            m_dspHost->onKeyDownSplit(decoder->getKeyOrController(), decoder->getValue(), determinedPart, interface);
          }
          else if(soundValid)
          {
            m_dspHost->onKeyDown(decoder->getKeyOrController(), decoder->getValue(), interface);
          }
        }
        if(m_options->shouldSendNotes() && soundValid)
          convertToAndSendMIDI(decoder, determinedPart);

        break;
      case DecoderEventType::KeyUp:
        if(m_options->shouldReceiveLocalNotes())
        {
          if(soundType == SoundType::Split)
          {
            determinedPart = calculateSplitPartForEvent(interface, decoder->getKeyOrController());
            m_dspHost->onKeyUpSplit(decoder->getKeyOrController(), decoder->getValue(), determinedPart, interface);
          }
          else if(soundValid)
          {
            // Single/Layer Sound acts global (maybe, only primary is preferred?)
            m_dspHost->onKeyUp(decoder->getKeyOrController(), decoder->getValue(), interface);
          }
        }
        if(m_options->shouldSendNotes() && soundValid)
          convertToAndSendMIDI(decoder, determinedPart);

        break;
      case DecoderEventType::HardwareChange:
        onHWChanged(decoder->getKeyOrController(), decoder->getValue(), DSPInterface::HWChangeSource::TCD);

        break;
      case DecoderEventType::UNKNOWN:
        nltools_detailedAssertAlways(false, "Decoded Event should not have UNKNOWN Type");
    }
  }
}

void InputEventStage::onMIDIEvent(MIDIDecoder *decoder)
{
  if(decoder)
  {
    const auto soundType = m_dspHost->getType();
    const bool soundValid = soundType != SoundType::Invalid;

    switch(decoder->getEventType())
    {
      case DecoderEventType::KeyDown:
      case DecoderEventType::KeyUp:
        if(checkMIDIKeyEventEnabled(decoder))
        {
          auto interface = getInterfaceFromParsedChannel(decoder->getChannel());
          if(interface.m_source == DSPInterface::InputSource::Unknown)
            return;

          const auto receivedOnPrimary = interface.m_source == DSPInterface::InputSource::Primary;

          if(soundType == SoundType::Split)
          {
            auto determinedPart = calculateSplitPartForEvent(interface, decoder->getKeyOrControl());
            if(decoder->getEventType() == DecoderEventType::KeyDown)
              m_dspHost->onKeyDownSplit(decoder->getKeyOrControl(), decoder->getValue(), determinedPart, interface);
            else if(decoder->getEventType() == DecoderEventType::KeyUp)
              m_dspHost->onKeyUpSplit(decoder->getKeyOrControl(), decoder->getValue(), determinedPart, interface);
          }
          else if(soundValid && receivedOnPrimary)
          {
            if(decoder->getEventType() == DecoderEventType::KeyUp)
              m_dspHost->onKeyUp(decoder->getKeyOrControl(), decoder->getValue(), interface);
            else if(decoder->getEventType() == DecoderEventType::KeyDown)
              m_dspHost->onKeyDown(decoder->getKeyOrControl(), decoder->getValue(), interface);
          }
        }
        break;

      case DecoderEventType::HardwareChange:
        if(checkMIDIHardwareChangeChannelMatches(decoder))
          onHWChanged(decoder->getKeyOrControl(), decoder->getValue(), DSPInterface::HWChangeSource::MIDI);
        break;

      case DecoderEventType::UNKNOWN:
        nltools_detailedAssertAlways(false, "Decoded Event should not have UNKNOWN Type");
    }
  }
}

void InputEventStage::convertToAndSendMIDI(TCDDecoder *pDecoder, const VoiceGroup &determinedPart)
{
  switch(pDecoder->getEventType())
  {
    case DecoderEventType::KeyDown:
      sendKeyDownAsMidi(pDecoder, determinedPart);
      break;
    case DecoderEventType::KeyUp:
      sendKeyUpAsMidi(pDecoder, determinedPart);
      break;
    case DecoderEventType::HardwareChange:
      sendHardwareChangeAsMidi(pDecoder->getKeyOrController(), pDecoder->getValue());
      break;
    case DecoderEventType::UNKNOWN:
      nltools_assertNotReached();
  }
}

bool InputEventStage::checkMIDIKeyEventEnabled(MIDIDecoder *pDecoder)
{
  using MRO = MidiRuntimeOptions;
  using MRC = MidiReceiveChannel;
  using MRCS = MidiReceiveChannelSplit;
  const auto shouldReceiveNotes = m_options->shouldReceiveNotes();
  const auto primaryChannel = m_options->getReceiveChannel();
  const auto secondaryChannel = m_options->getReceiveSplitChannel();

  const auto parsedChannel = pDecoder->getChannel();

  const auto primNotNone = primaryChannel != MRC::None;
  const auto secNotNone = secondaryChannel != MRCS::None;
  const auto primaryChannelMatches = parsedChannel == primaryChannel || primaryChannel == MRC::Omni;
  const auto secondaryChannelMatches = MRO::normalToSplitChannel(parsedChannel) == secondaryChannel
      || secondaryChannel == MRCS::Omni || secondaryChannel == MRCS::Common;
  const auto channelMatches = primaryChannelMatches || secondaryChannelMatches;
  return shouldReceiveNotes && channelMatches && (primNotNone || secNotNone);
}

bool InputEventStage::checkMIDIHardwareChangeChannelMatches(MIDIDecoder *pDecoder)
{
  const auto channelMatches = m_options->getReceiveChannel() == MidiReceiveChannel::Omni
      || pDecoder->getChannel() == m_options->getReceiveChannel();
  return channelMatches;
}

void InputEventStage::sendKeyDownAsMidi(TCDDecoder *pDecoder, const VoiceGroup &determinedPart)
{
  using CC_Range_Vel = Midi::clipped14BitVelRange;

  const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getSendChannel());
  const auto secondaryChannel = m_options->channelEnumToInt(m_options->getSendSplitChannel());
  const auto key = pDecoder->getKeyOrController();

  if(mainChannel != -1 && (determinedPart == VoiceGroup::I || determinedPart == VoiceGroup::Global))
  {
    auto mainC = static_cast<uint8_t>(mainChannel);

    constexpr const uint8_t ccType = 0xB0;
    const uint8_t lsbStatus = ccType | mainC;

    uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(pDecoder->getValue());
    auto lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
    m_midiOut({ lsbStatus, 88, lsbVelByte });

    constexpr const uint8_t keyType = 0x90;
    const uint8_t keyStatus = keyType | mainC;
    uint8_t keyByte = static_cast<uint8_t>(key) & 0x7F;
    auto msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
    m_midiOut({ keyStatus, keyByte, msbVelByte });
  }

  if(secondaryChannel != -1 && (determinedPart == VoiceGroup::II || determinedPart == VoiceGroup::Global)
     && m_dspHost->getType() == SoundType::Split)
  {
    auto secC = static_cast<uint8_t>(secondaryChannel);

    constexpr const uint8_t ccType = 0xB0;
    const uint8_t lsbStatus = ccType | secC;

    uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(pDecoder->getValue());
    auto lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
    m_midiOut({ lsbStatus, 88, lsbVelByte });

    constexpr const uint8_t keyType = 0x90;
    const uint8_t keyStatus = keyType | secC;
    uint8_t keyByte = static_cast<uint8_t>(key) & 0x7F;
    auto msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
    m_midiOut({ keyStatus, keyByte, msbVelByte });
  }
}

void InputEventStage::sendKeyUpAsMidi(TCDDecoder *pDecoder, const VoiceGroup &determinedPart)
{
  // key up events should include part evaluation (as key down events)
  using CC_Range_Vel = Midi::clipped14BitVelRange;

  const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getSendChannel());
  const auto secondaryChannel = m_options->channelEnumToInt(m_options->getSendSplitChannel());
  const auto key = pDecoder->getKeyOrController();

  const auto mainC = static_cast<uint8_t>(mainChannel);
  const auto secC = static_cast<uint8_t>(secondaryChannel);
  constexpr const uint8_t keyType = 0x80;
  constexpr const uint8_t ccType = 0xB0;

  if(mainChannel != -1 && (determinedPart == VoiceGroup::I || determinedPart == VoiceGroup::Global))
  {
    const uint8_t lsbStatus = ccType | mainC;

    uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(pDecoder->getValue());
    auto lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
    m_midiOut({ lsbStatus, 88, lsbVelByte });

    const uint8_t keyStatus = keyType | mainC;
    uint8_t keyByte = static_cast<uint8_t>(key) & 0x7F;
    auto msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
    m_midiOut({ keyStatus, keyByte, msbVelByte });
  }

  if(secondaryChannel != -1 && (determinedPart == VoiceGroup::II || determinedPart == VoiceGroup::Global)
     && m_dspHost->getType() == SoundType::Split)
  {

    const uint8_t lsbStatus = ccType | secC;

    uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(pDecoder->getValue());
    auto lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
    m_midiOut({ lsbStatus, 88, lsbVelByte });

    const uint8_t keyStatus = keyType | secC;
    uint8_t keyByte = static_cast<uint8_t>(key) & 0x7F;
    auto msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
    m_midiOut({ keyStatus, keyByte, msbVelByte });
  }
}

void InputEventStage::sendHardwareChangeAsMidi(int hwID, float value)
{
  auto roundPedalToSwitching = [](float val) -> float {
    if(val >= 0.5f)
      return 1.0f;
    else
      return 0.0f;
  };

  switch(hwID)
  {
    case 0:
      if(m_options->isSwitchingCC(0))
        value = roundPedalToSwitching(value);
      sendCCOut(0, value, m_options->getCCFor<Midi::MSB::Ped1>(), m_options->getCCFor<Midi::LSB::Ped1>());
      break;
    case 1:
      if(m_options->isSwitchingCC(1))
        value = roundPedalToSwitching(value);
      sendCCOut(1, value, m_options->getCCFor<Midi::MSB::Ped2>(), m_options->getCCFor<Midi::LSB::Ped2>());
      break;
    case 2:
      if(m_options->isSwitchingCC(2))
        value = roundPedalToSwitching(value);
      sendCCOut(2, value, m_options->getCCFor<Midi::MSB::Ped3>(), m_options->getCCFor<Midi::LSB::Ped3>());
      break;
    case 3:
      if(m_options->isSwitchingCC(3))
        value = roundPedalToSwitching(value);
      sendCCOut(3, value, m_options->getCCFor<Midi::MSB::Ped4>(), m_options->getCCFor<Midi::LSB::Ped4>());
      break;
    case 4:
      doSendBenderOut(value);
      break;
    case 5:
      doSendAftertouchOut(value);
      break;
    case 6:
      sendCCOut(6, value, m_options->getCCFor<Midi::MSB::Rib1>(), m_options->getCCFor<Midi::LSB::Rib1>());
      break;
    case 7:
      sendCCOut(7, value, m_options->getCCFor<Midi::MSB::Rib2>(), m_options->getCCFor<Midi::LSB::Rib2>());
      break;
    default:
      break;
  }
}

void InputEventStage::sendCCOut(int hwID, float value, int msbCC, int lsbCC)
{
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;

  //CHECK if pedal is switching and only send MSB message
  if(m_dspHost->getBehaviour(hwID) == C15::Properties::HW_Return_Behavior::Center)
    doSendCCOut(CC_Range_14_Bit::encodeBipolarMidiValue(value), msbCC, lsbCC);
  else
    doSendCCOut(CC_Range_14_Bit::encodeUnipolarMidiValue(value), msbCC, lsbCC);
}

void InputEventStage::doSendCCOut(uint16_t value, int msbCC, int lsbCC)
{
  const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getSendChannel());
  const auto secondaryChannel = m_options->channelEnumToInt(m_options->getSendSplitChannel());

  const auto mainC = static_cast<uint8_t>(mainChannel);
  const auto secC = static_cast<uint8_t>(secondaryChannel);

  auto statusByte = static_cast<uint8_t>(0xB0);
  auto lsbValByte = static_cast<uint8_t>(value & 0x7F);
  auto msbValByte = static_cast<uint8_t>(value >> 7 & 0x7F);

  if(mainChannel != -1)
  {
    auto mainStatus = static_cast<uint8_t>(statusByte | mainC);
    if(lsbCC != -1)
      m_midiOut({ mainStatus, static_cast<uint8_t>(lsbCC), lsbValByte });

    m_midiOut({ mainStatus, static_cast<uint8_t>(msbCC), msbValByte });
  }

  if(secondaryChannel != -1 && m_dspHost->getType() == SoundType::Split)
  {
    auto secStatus = static_cast<uint8_t>(statusByte | secC);
    if(lsbCC != -1)
      m_midiOut({ secStatus, static_cast<uint8_t>(lsbCC), lsbValByte });

    m_midiOut({ secStatus, static_cast<uint8_t>(msbCC), msbValByte });
  }
}

void InputEventStage::setNoteShift(int i)
{
  m_shifteable_keys.setNoteShift(i);
}

VoiceGroup InputEventStage::calculateSplitPartForEvent(DSPInterface::InputEvent inputEvent, const int keyNumber)
{
  const auto useSplitPoint = inputEvent.m_state == DSPInterface::InputState::Singular
      || inputEvent.m_source == DSPInterface::InputSource::TCD;

  if(useSplitPoint)
    return m_dspHost->getSplitPartForKey(keyNumber);

  switch(inputEvent.m_source)
  {
    case DSPInterface::InputSource::Primary:  // handles separate Primary
      return VoiceGroup::I;
    case DSPInterface::InputSource::Secondary:  // handles separate Secondary
      return VoiceGroup::II;
    case DSPInterface::InputSource::Both:
      return VoiceGroup::Global;
    case DSPInterface::InputSource::Unknown:
    case DSPInterface::InputSource::TCD:
      nltools_assertNotReached();
  }
  nltools_assertNotReached();
}

DSPInterface::InputEvent InputEventStage::getInterfaceFromParsedChannel(MidiReceiveChannel channel)
{
  const auto primary = m_options->getReceiveChannel();
  const auto secondary = m_options->getReceiveSplitChannel();
  const auto primaryMask = midiReceiveChannelMask(primary);
  const auto secondaryMask = midiReceiveChannelMask(secondary);
  // first check: did event qualify at all?
  const bool qualifiedForEvent = (primaryMask | secondaryMask) > 0;
  if(!qualifiedForEvent)
    return InputStateDetail::Unknown;
  // second check: obtain channel mask and evaluate common "singular" case
  const auto channelMask = midiReceiveChannelMask(channel);
  const bool qualifiedForCommon = secondary == MidiReceiveChannelSplit::Common;
  const bool qualifiedForPrimary = (channelMask & primaryMask) > 0;
  if(qualifiedForCommon)
    return qualifiedForPrimary ? InputStateDetail::Singular : InputStateDetail::Unknown;
  // third checks: evaluate "separate" cases (both, primary, secondary)
  const bool qualifiedForSecondary = (channelMask & secondaryMask) > 0;
  const bool qualifiedForIdentity = qualifiedForPrimary && qualifiedForSecondary;
  if(qualifiedForIdentity)
    return InputStateDetail::Both;
  if(qualifiedForPrimary)
    return InputStateDetail::Primary;
  if(qualifiedForSecondary)
    return InputStateDetail::Secondary;
  // this line should never be reached
  return InputStateDetail::Unknown;
}

void InputEventStage::doSendAftertouchOut(float value)
{
  using CC_Range_7_Bit = Midi::FullCCRange<Midi::Formats::_7_Bits_>;
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;

  const auto sendAsCC = m_options->getAftertouchLSBCC().first;
  if(sendAsCC)
  {
    doSendCCOut(CC_Range_14_Bit::encodeUnipolarMidiValue(value), m_options->getAftertouchMSBCC().second,
                m_options->getAftertouchLSBCC().second);
  }
  else if(m_options->getAftertouchSetting() == AftertouchCC::ChannelPressure)
  {
    const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getSendChannel());
    const auto secondaryChannel = m_options->channelEnumToInt(m_options->getSendSplitChannel());

    const auto mainC = static_cast<uint8_t>(mainChannel);
    const auto secC = static_cast<uint8_t>(secondaryChannel);

    auto atStatusByte = static_cast<uint8_t>(0xD0);
    uint8_t valByte = CC_Range_7_Bit::encodeUnipolarMidiValue(value);

    if(mainChannel != -1)
    {
      auto mainStatus = static_cast<uint8_t>(atStatusByte | mainC);
      m_midiOut({ mainStatus, valByte });
    }

    if(secondaryChannel != -1 && m_dspHost->getType() == SoundType::Split)
    {
      auto secStatus = static_cast<uint8_t>(atStatusByte | secC);
      m_midiOut({ secStatus, valByte });
    }
  }
  else if(m_options->getAftertouchSetting() == AftertouchCC::PitchbendDown
          || m_options->getAftertouchSetting() == AftertouchCC::PitchbendUp)
  {
    if(m_options->getAftertouchSetting() == AftertouchCC::PitchbendDown)
      value *= -1.f;

    using CC_Range_Bender = Midi::FullCCRange<Midi::Formats::_14_Bits_>;

    auto v = CC_Range_Bender::encodeBipolarMidiValue(value);
    auto lsb = static_cast<uint8_t>(v & 0x7F);
    auto msb = static_cast<uint8_t>((v >> 7) & 0x7F);

    const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getSendChannel());
    const auto secondaryChannel = m_options->channelEnumToInt(m_options->getSendSplitChannel());

    const auto mainC = static_cast<uint8_t>(mainChannel);
    const auto secC = static_cast<uint8_t>(secondaryChannel);

    auto statusByte = static_cast<uint8_t>(0xE0);

    if(mainChannel != -1)
    {
      auto mainStatus = static_cast<uint8_t>(statusByte | mainC);
      m_midiOut({ mainStatus, lsb, msb });
    }

    if(secondaryChannel != -1 && m_dspHost->getType() == SoundType::Split)
    {
      auto secStatus = static_cast<uint8_t>(statusByte | secC);
      m_midiOut({ secStatus, lsb, msb });
    }
  }
}

void InputEventStage::doSendBenderOut(float value)
{
  const auto sendAsCC = m_options->getBenderLSBCC().first;
  if(!sendAsCC)
  {
    using CC_Range_Bender = Midi::FullCCRange<Midi::Formats::_14_Bits_>;

    auto v = CC_Range_Bender::encodeBipolarMidiValue(value);
    auto lsb = static_cast<uint8_t>(v & 0x7F);
    auto msb = static_cast<uint8_t>((v >> 7) & 0x7F);

    const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getSendChannel());
    const auto secondaryChannel = m_options->channelEnumToInt(m_options->getSendSplitChannel());

    const auto mainC = static_cast<uint8_t>(mainChannel);
    const auto secC = static_cast<uint8_t>(secondaryChannel);

    auto statusByte = static_cast<uint8_t>(0xE0);

    if(mainChannel != -1)
    {
      auto mainStatus = static_cast<uint8_t>(statusByte | mainC);
      m_midiOut({ mainStatus, lsb, msb });
    }

    if(secondaryChannel != -1 && m_dspHost->getType() == SoundType::Split)
    {
      auto secStatus = static_cast<uint8_t>(statusByte | secC);
      m_midiOut({ secStatus, lsb, msb });
    }
  }
  else
  {
    using CC_Range_14_Bit = Midi::clipped14BitCCRange;
    auto lsbCC = m_options->getBenderLSBCC().second;
    auto msbCC = m_options->getBenderMSBCC().second;
    doSendCCOut(CC_Range_14_Bit::encodeBipolarMidiValue(value), msbCC, lsbCC);
  }
}

void InputEventStage::onUIHWSourceMessage(const nltools::msg::HWSourceChangedMessage &message)
{
  auto hwID = InputEventStage::parameterIDToHWID(message.parameterId);

  if(hwID != -1)
  {
    onHWChanged(hwID, message.controlPosition, DSPInterface::HWChangeSource::UI);
  }
}

int InputEventStage::parameterIDToHWID(int id)
{
  switch(id)
  {
    case C15::PID::Pedal_1:
      return 0;
    case C15::PID::Pedal_2:
      return 1;
    case C15::PID::Pedal_3:
      return 2;
    case C15::PID::Pedal_4:
      return 3;
    case C15::PID::Bender:
      return 4;
    case C15::PID::Aftertouch:
      return 5;
    case C15::PID::Ribbon_1:
      return 6;
    case C15::PID::Ribbon_2:
      return 7;
    default:
      return -1;
  }
}

int InputEventStage::HWIDToParameterID(int id)
{
  switch(id)
  {
    case 0:
      return C15::PID::Pedal_1;
    case 1:
      return C15::PID::Pedal_2;
    case 2:
      return C15::PID::Pedal_3;
    case 3:
      return C15::PID::Pedal_4;
    case 4:
      return C15::PID::Bender;
    case 5:
      return C15::PID::Aftertouch;
    case 6:
      return C15::PID::Ribbon_1;
    case 7:
      return C15::PID::Ribbon_2;
    default:
      return -1;
  }
}

void InputEventStage::onHWChanged(int hwID, float pos, DSPInterface::HWChangeSource source)
{
  auto sendToDSP = false;

  switch(source)
  {
    case DSPInterface::HWChangeSource::TCD:
      sendToDSP = m_options->shouldReceiveLocalControllers();
      break;
    case DSPInterface::HWChangeSource::MIDI:
      sendToDSP = m_options->shouldReceiveMIDIControllers();
      break;
    case DSPInterface::HWChangeSource::UI:
      sendToDSP = true;
      break;
  }

  if(sendToDSP)
  {
    m_dspHost->onHWChanged(hwID, pos);
  }

  if(source != DSPInterface::HWChangeSource::MIDI && m_options->shouldSendControllers())
  {
    sendHardwareChangeAsMidi(hwID, pos);
  }

  if(source == DSPInterface::HWChangeSource::MIDI && m_options->shouldReceiveMIDIControllers())
  {
    nltools::Log::warning("received HWChange via MIDI! for hwID:", hwID);
    auto parameterID = HWIDToParameterID(hwID);
    if(parameterID != -1)
    {
      nltools::Log::warning("sending HWChange that was received via MIDI! value:", pos, "id:", parameterID);
      nltools::msg::Midi::HardwareChangeMessage msg {};
      msg.parameterID = parameterID;
      msg.value = pos;
      nltools::msg::send(nltools::msg::EndPoint::Playground, msg);
    }
  }
}