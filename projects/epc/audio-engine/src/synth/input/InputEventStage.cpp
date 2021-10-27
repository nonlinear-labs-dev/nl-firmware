#include <utility>
#include <MidiRuntimeOptions.h>
#include "InputEventStage.h"
#include "synth/c15-audio-engine/dsp_host_dual.h"

InputEventStage::InputEventStage(DSPInterface *dspHost, MidiRuntimeOptions *options, HWChangedNotification hwChangedCB,
                                 InputEventStage::MIDIOut outCB, InputEventStage::ChannelModeMessageCB specialCB)
    : m_tcdDecoder(dspHost, options, &m_shifteable_keys)
    , m_midiDecoder(dspHost, options)
    , m_dspHost { dspHost }
    , m_options { options }
    , m_hwChangedCB(std::move(hwChangedCB))
    , m_channelModeMessageCB(std::move(specialCB))
    , m_midiOut { std::move(outCB) }
{
  std::fill(m_latchedHWPositions.begin(), m_latchedHWPositions.end(),
            std::array<uint16_t, 2> { std::numeric_limits<uint16_t>::max(), std::numeric_limits<uint16_t>::max() });
}

template <>
bool InputEventStage::latchHWPosition<InputEventStage::LatchMode::LSBAndMSB>(HardwareSource hwID, uint8_t lsb,
                                                                             uint8_t msb)
{
  auto didChange = false;
  const auto idx = static_cast<int>(hwID);
  auto &latchedPos = m_latchedHWPositions[idx];

  if(latchedPos[0] != lsb)
  {
    latchedPos[0] = lsb;
    didChange = true;
  }
  if(latchedPos[1] != msb)
  {
    latchedPos[1] = msb;
    didChange = true;
  }

  return didChange;
}

template <>
bool InputEventStage::latchHWPosition<InputEventStage::LatchMode::OnlyMSB>(HardwareSource hwID, uint8_t lsb,
                                                                           uint8_t msb)
{
  auto didChange = false;
  const auto idx = static_cast<int>(hwID);
  auto &latchedPos = m_latchedHWPositions[idx];

  if(latchedPos[1] != msb)
  {
    latchedPos[1] = msb;
    didChange = true;
  }

  return didChange;
}

template <>
bool InputEventStage::latchHWPosition<InputEventStage::LatchMode::Option>(HardwareSource hwID, uint8_t lsb, uint8_t msb)
{
  if(m_options->is14BitSupportEnabled())
  {
    return latchHWPosition<LatchMode::LSBAndMSB>(hwID, lsb, msb);
  }
  else
  {
    return latchHWPosition<LatchMode::OnlyMSB>(hwID, lsb, msb);
  }
}

void InputEventStage::onTCDMessage(const MidiEvent &tcdEvent)
{
  if(m_tcdDecoder.decode(tcdEvent))
  {
    onTCDEvent();
    m_tcdDecoder.reset();
  }
}

void InputEventStage::onMIDIMessage(const MidiEvent &midiEvent)
{
  if(m_midiDecoder.decode(midiEvent))
  {
    onMIDIEvent();
    m_midiDecoder.reset();
  }
}

void InputEventStage::onTCDEvent()
{
  auto decoder = &m_tcdDecoder;
  const SoundType soundType = m_dspHost->getType();
  const bool soundValid = soundType != SoundType::Invalid;
  const auto interface = DSPInterface::InputEventSource::Internal;

  switch(decoder->getEventType())
  {
    case DecoderEventType::KeyDown:
    {
      const bool isSplitSound = (soundType == SoundType::Split);
      const auto determinedPart
          = isSplitSound ? calculateSplitPartForKeyDown(interface, decoder->getKeyOrController()) : VoiceGroup::Global;

      if(determinedPart == VoiceGroup::Invalid)
        break;

      if(m_options->shouldReceiveLocalNotes())
      {
        if(isSplitSound)
        {
          m_dspHost->onKeyDownSplit(decoder->getKeyOrController(), decoder->getValue(), determinedPart, interface);
        }
        else if(soundValid)
        {
          m_dspHost->onKeyDown(decoder->getKeyOrController(), decoder->getValue(), interface);
        }

        setAndScheduleKeybedNotify();
      }
      else if(isSplitSound)
      {
        // despite the suppression of internal notes due to local off,
        // we still need to latch the determined part for subsequent key up events (unfortunate, but not avoidable)
        m_dspHost->registerNonLocalSplitKeyAssignment(decoder->getKeyOrController(), determinedPart, interface);
      }
      if((m_options->shouldSendMIDINotesOnSplit() || m_options->shouldSendMIDINotesOnPrimary()) && soundValid)
      {
        convertToAndSendMIDI(decoder, determinedPart);
      }

      break;
    }
    case DecoderEventType::KeyUp:
    {
      const bool isSplitSound = (soundType == SoundType::Split);
      const auto determinedPart
          = isSplitSound ? calculateSplitPartForKeyUp(interface, decoder->getKeyOrController()) : VoiceGroup::Global;

      if(determinedPart == VoiceGroup::Invalid)
        break;

      if(m_options->shouldReceiveLocalNotes())
      {
        if(isSplitSound)
        {
          m_dspHost->onKeyUpSplit(decoder->getKeyOrController(), decoder->getValue(), determinedPart, interface);
        }
        else if(soundValid)
        {
          m_dspHost->onKeyUp(decoder->getKeyOrController(), decoder->getValue(), interface);
        }

        setAndScheduleKeybedNotify();
      }
      else if(isSplitSound)
      {
        // despite the suppression of internal notes due to local off,
        // we still want to clear the key assingment
        m_dspHost->unregisterNonLocalSplitKeyAssignment(decoder->getKeyOrController(), determinedPart, interface);
      }
      if((m_options->shouldSendMIDINotesOnSplit() || m_options->shouldSendMIDINotesOnPrimary()) && soundValid)
      {
        convertToAndSendMIDI(decoder, determinedPart);
      }

      break;
    }

    case DecoderEventType::HardwareChange:
      onHWChanged(static_cast<HardwareSource>(decoder->getKeyOrController()), decoder->getValue(), HWChangeSource::TCD,
                  false, false, false);

      break;
    case DecoderEventType::UNKNOWN:
      nltools_detailedAssertAlways(false, "Decoded Event should not have UNKNOWN Type");
  }
}

void InputEventStage::onMIDIEvent()
{
  auto decoder = &m_midiDecoder;
  const auto soundType = m_dspHost->getType();
  const bool soundValid = soundType != SoundType::Invalid;

  switch(decoder->getEventType())
  {
    case DecoderEventType::KeyDown:
    case DecoderEventType::KeyUp:
      if(checkMIDIKeyEventEnabled(decoder))
      {
        auto inputSource = getInputSourceFromParsedChannel(decoder->getChannel());
        if(inputSource == DSPInterface::InputEventSource::Invalid)
          return;

        const auto receivedOnSecondary = inputSource == DSPInterface::InputEventSource::External_Secondary;

        if(soundType == SoundType::Split)
        {
          if(decoder->getEventType() == DecoderEventType::KeyDown)
          {
            auto determinedPart = calculateSplitPartForKeyDown(inputSource, decoder->getKeyOrControl());
            m_dspHost->onKeyDownSplit(decoder->getKeyOrControl(), decoder->getValue(), determinedPart, inputSource);
          }
          else if(decoder->getEventType() == DecoderEventType::KeyUp)
          {
            auto determinedPart = calculateSplitPartForKeyUp(inputSource, decoder->getKeyOrControl());
            m_dspHost->onKeyUpSplit(decoder->getKeyOrControl(), decoder->getValue(), determinedPart, inputSource);
          }

          setAndScheduleKeybedNotify();
        }
        else if(soundValid && !receivedOnSecondary)
        {
          if(decoder->getEventType() == DecoderEventType::KeyUp)
            m_dspHost->onKeyUp(decoder->getKeyOrControl(), decoder->getValue(), inputSource);
          else if(decoder->getEventType() == DecoderEventType::KeyDown)
            m_dspHost->onKeyDown(decoder->getKeyOrControl(), decoder->getValue(), inputSource);

          setAndScheduleKeybedNotify();
        }
      }
      break;

    case DecoderEventType::HardwareChange:
      if(checkMIDIHardwareChangeChannelMatches(decoder))
        onMIDIHWChanged(decoder);
      break;

    case DecoderEventType::UNKNOWN:
      nltools_detailedAssertAlways(false, "Decoded Event should not have UNKNOWN Type");
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
      sendHardwareChangeAsMidi(static_cast<HardwareSource>(pDecoder->getKeyOrController()), pDecoder->getValue());
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

  const auto primaryChannel = m_options->getMIDIPrimaryReceiveChannel();
  const auto secondaryChannel = m_options->getMIDISplitReceiveChannel();

  const auto parsedChannel = pDecoder->getChannel();

  const auto primNotNone = primaryChannel != MRC::None;
  const auto secNotNone = secondaryChannel != MRCS::None;

  const auto primaryChannelMatches = parsedChannel == primaryChannel || primaryChannel == MRC::Omni;
  const auto secondaryChannelMatches = MRO::normalToSplitChannel(parsedChannel) == secondaryChannel
      || secondaryChannel == MRCS::Omni || secondaryChannel == MRCS::Common;

  const auto primaryEnabledAndMatched = primaryChannelMatches && m_options->shouldReceiveMIDINotesOnPrimary();
  const auto secondaryEnabledAndMatched = secondaryChannelMatches && m_options->shouldReceiveMIDINotesOnSplit();
  const auto channelMatchesAndAllowed = primaryEnabledAndMatched || secondaryEnabledAndMatched;
  return channelMatchesAndAllowed && (primNotNone || secNotNone);
}

bool InputEventStage::checkMIDIHardwareChangeChannelMatches(MIDIDecoder *pDecoder)
{
  const auto channelMatches = m_options->getMIDIPrimaryReceiveChannel() == MidiReceiveChannel::Omni
      || pDecoder->getChannel() == m_options->getMIDIPrimaryReceiveChannel();

  const auto splitMatches = m_options->getMIDISplitReceiveChannel() == MidiReceiveChannelSplit::Omni
      || MidiRuntimeOptions::normalToSplitChannel(pDecoder->getChannel()) == m_options->getMIDISplitReceiveChannel();
  return channelMatches || (isSplitDSP() && splitMatches);
}

void InputEventStage::sendKeyDownAsMidi(TCDDecoder *pDecoder, const VoiceGroup &determinedPart)
{
  const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDIPrimarySendChannel());
  const auto secondaryChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDISplitSendChannel());
  const auto key = pDecoder->getKeyOrController();
  constexpr const uint8_t keyType = 0x90;
  constexpr const uint8_t ccType = 0xB0;
  const uint8_t keyByte = static_cast<uint8_t>(key) & 0x7F;

  if(mainChannel != -1
     && ((determinedPart == VoiceGroup::I || determinedPart == VoiceGroup::Global)
         || m_options->getMIDISplitSendChannel() == MidiSendChannelSplit::Common)
     && m_options->shouldSendMIDINotesOnPrimary())
  {
    auto mainC = static_cast<uint8_t>(mainChannel);
    const uint8_t keyStatus = keyType | mainC;

    if(m_options->enableHighVelCC())
    {
      uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(pDecoder->getValue());

      const uint8_t lsbStatus = ccType | mainC;
      auto lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
      m_midiOut({ lsbStatus, 88, lsbVelByte });

      auto msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
      m_midiOut({ keyStatus, keyByte, msbVelByte });
    }
    else
    {
      auto stdVal = static_cast<uint8_t>(CC_Range_7_Bit::encodeUnipolarMidiValue(pDecoder->getValue()));
      m_midiOut({ keyStatus, keyByte, stdVal });
    }

    if(m_options->getMIDISplitSendChannel() == MidiSendChannelSplit::Common)
      return;
  }

  if(secondaryChannel != -1 && (determinedPart == VoiceGroup::II || determinedPart == VoiceGroup::Global)
     && m_dspHost->getType() == SoundType::Split && m_options->shouldSendMIDINotesOnSplit())
  {
    auto secC = static_cast<uint8_t>(secondaryChannel);
    const uint8_t keyStatus = keyType | secC;

    if(m_options->enableHighVelCC())
    {
      uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(pDecoder->getValue());

      const uint8_t lsbStatus = ccType | secC;
      auto lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
      m_midiOut({ lsbStatus, 88, lsbVelByte });

      auto msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
      m_midiOut({ keyStatus, keyByte, msbVelByte });
    }
    else
    {
      auto stdVal = static_cast<uint8_t>(CC_Range_7_Bit::encodeUnipolarMidiValue(pDecoder->getValue()));
      m_midiOut({ keyStatus, keyByte, stdVal });
    }
  }
}

void InputEventStage::sendKeyUpAsMidi(TCDDecoder *pDecoder, const VoiceGroup &determinedPart)
{
  const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDIPrimarySendChannel());
  const auto secondaryChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDISplitSendChannel());
  const auto key = pDecoder->getKeyOrController();

  const auto mainC = static_cast<uint8_t>(mainChannel);
  const auto secC = static_cast<uint8_t>(secondaryChannel);
  constexpr const uint8_t keyType = 0x80;
  constexpr const uint8_t ccType = 0xB0;

  if(mainChannel != -1
     && ((determinedPart == VoiceGroup::I || determinedPart == VoiceGroup::Global)
         || m_options->getMIDISplitSendChannel() == MidiSendChannelSplit::Common)
     && m_options->shouldSendMIDINotesOnPrimary())
  {
    const uint8_t keyStatus = keyType | mainC;
    uint8_t keyByte = static_cast<uint8_t>(key) & 0x7F;

    if(m_options->enableHighVelCC())
    {
      const uint8_t lsbStatus = ccType | mainC;
      uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(pDecoder->getValue());
      auto lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
      m_midiOut({ lsbStatus, 88, lsbVelByte });

      auto msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
      m_midiOut({ keyStatus, keyByte, msbVelByte });
    }
    else
    {
      auto stdValue = static_cast<uint8_t>(CC_Range_7_Bit::encodeUnipolarMidiValue(pDecoder->getValue()));
      m_midiOut({ keyStatus, keyByte, stdValue });
    }

    if(m_options->getMIDISplitSendChannel() == MidiSendChannelSplit::Common)
      return;
  }

  if(secondaryChannel != -1 && (determinedPart == VoiceGroup::II || determinedPart == VoiceGroup::Global)
     && m_dspHost->getType() == SoundType::Split && m_options->shouldSendMIDINotesOnSplit())
  {
    const uint8_t keyStatus = keyType | secC;
    uint8_t keyByte = static_cast<uint8_t>(key) & 0x7F;

    if(m_options->enableHighVelCC())
    {
      const uint8_t lsbStatus = ccType | secC;
      uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(pDecoder->getValue());
      auto lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
      m_midiOut({ lsbStatus, 88, lsbVelByte });

      auto msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
      m_midiOut({ keyStatus, keyByte, msbVelByte });
    }
    else
    {
      auto stdValue = static_cast<uint8_t>(CC_Range_7_Bit::encodeUnipolarMidiValue(pDecoder->getValue()));
      m_midiOut({ keyStatus, keyByte, stdValue });
    }
  }
}

void InputEventStage::sendHardwareChangeAsMidi(HardwareSource hwID, float value)
{
  switch(hwID)
  {
    case HardwareSource::PEDAL1:
      sendCCOut(HardwareSource::PEDAL1, value, m_options->getCCFor<Midi::MSB::Ped1>(),
                m_options->getCCFor<Midi::LSB::Ped1>());
      break;

    case HardwareSource::PEDAL2:
      sendCCOut(HardwareSource::PEDAL2, value, m_options->getCCFor<Midi::MSB::Ped2>(),
                m_options->getCCFor<Midi::LSB::Ped2>());
      break;

    case HardwareSource::PEDAL3:
      sendCCOut(HardwareSource::PEDAL3, value, m_options->getCCFor<Midi::MSB::Ped3>(),
                m_options->getCCFor<Midi::LSB::Ped3>());
      break;

    case HardwareSource::PEDAL4:
      sendCCOut(HardwareSource::PEDAL4, value, m_options->getCCFor<Midi::MSB::Ped4>(),
                m_options->getCCFor<Midi::LSB::Ped4>());
      break;

    case HardwareSource::BENDER:
      doSendBenderOut(value);
      break;

    case HardwareSource::AFTERTOUCH:
      doSendAftertouchOut(value);
      break;

    case HardwareSource::RIBBON1:
      sendCCOut(HardwareSource::RIBBON1, value, m_options->getCCFor<Midi::MSB::Rib1>(),
                m_options->getCCFor<Midi::LSB::Rib1>());
      break;

    case HardwareSource::RIBBON2:
      sendCCOut(HardwareSource::RIBBON2, value, m_options->getCCFor<Midi::MSB::Rib2>(),
                m_options->getCCFor<Midi::LSB::Rib2>());
      break;

    default:
      break;
  }
}

void InputEventStage::sendCCOut(HardwareSource hwID, float value, int msbCC, int lsbCC)
{
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;

  if(m_dspHost->getBehaviour(hwID) == C15::Properties::HW_Return_Behavior::Center)
    doSendCCOut(CC_Range_14_Bit::encodeBipolarMidiValue(value), msbCC, lsbCC, hwID);
  else
    doSendCCOut(CC_Range_14_Bit::encodeUnipolarMidiValue(value), msbCC, lsbCC, hwID);
}

void InputEventStage::doSendCCOut(uint16_t value, int msbCC, int lsbCC, HardwareSource hwID)
{
  const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDIPrimarySendChannel());
  const auto secondaryChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDISplitSendChannel());

  const auto mainC = static_cast<uint8_t>(mainChannel);
  const auto secC = static_cast<uint8_t>(secondaryChannel);

  auto statusByte = static_cast<uint8_t>(0xB0);
  auto lsbValByte = static_cast<uint8_t>(value & 0x7F);
  auto msbValByte = static_cast<uint8_t>(value >> 7 & 0x7F);

  if(!latchHWPosition<LatchMode::Option>(hwID, lsbValByte, msbValByte))
    return;

  auto routingIndex = static_cast<RoutingIndex>(hwID);

  if(mainChannel != -1 && m_options->shouldSendMidiOnPrimary(routingIndex))
  {
    auto mainStatus = static_cast<uint8_t>(statusByte | mainC);

    if(lsbCC != -1 && m_options->is14BitSupportEnabled())
    {
      m_midiOut({ mainStatus, static_cast<uint8_t>(lsbCC), lsbValByte });
    }

    if(msbCC != -1)
    {
      m_midiOut({ mainStatus, static_cast<uint8_t>(msbCC), msbValByte });
    }
  }

  if(secondaryChannel != -1 && m_options->shouldSendMidiOnSplit(routingIndex) && isSplitDSP())
  {
    auto secStatus = static_cast<uint8_t>(statusByte | secC);

    if(lsbCC != -1 && m_options->is14BitSupportEnabled())
    {
      m_midiOut({ secStatus, static_cast<uint8_t>(lsbCC), lsbValByte });
    }

    if(msbCC != -1)
    {
      m_midiOut({ secStatus, static_cast<uint8_t>(msbCC), msbValByte });
    }
  }
}

void InputEventStage::setNoteShift(int i)
{
  m_shifteable_keys.setNoteShift(i);
}

int InputEventStage::getNoteShift() const
{
  return m_shifteable_keys.getNoteShift();
}

VoiceGroup InputEventStage::calculateSplitPartForKeyDown(DSPInterface::InputEventSource inputEvent, const int keyNumber)
{
  switch(inputEvent)
  {
    case DSPInterface::InputEventSource::Internal:
    case DSPInterface::InputEventSource::External_Use_Split:
      return m_dspHost->getSplitPartForKeyDown(keyNumber);
    case DSPInterface::InputEventSource::External_Primary:
      return VoiceGroup::I;
    case DSPInterface::InputEventSource::External_Secondary:
      return VoiceGroup::II;
    case DSPInterface::InputEventSource::External_Both:
      return VoiceGroup::Global;
    case DSPInterface::InputEventSource::Invalid:
      break;
  }
  nltools_assertNotReached();
}

VoiceGroup InputEventStage::calculateSplitPartForKeyUp(DSPInterface::InputEventSource inputEvent, const int keyNumber)
{
  switch(inputEvent)
  {
    case DSPInterface::InputEventSource::Internal:
    case DSPInterface::InputEventSource::External_Use_Split:
      return m_dspHost->getSplitPartForKeyUp(keyNumber, inputEvent);  //DEBUG: NumGroups gets returned here
    case DSPInterface::InputEventSource::External_Primary:
      return VoiceGroup::I;
    case DSPInterface::InputEventSource::External_Secondary:
      return VoiceGroup::II;
    case DSPInterface::InputEventSource::External_Both:
      return VoiceGroup::Global;
    case DSPInterface::InputEventSource::Invalid:
      nltools::Log::error(__PRETTY_FUNCTION__, "INVALID for keyNum:", keyNumber);
      break;
  }
  nltools_assertNotReached();
}

DSPInterface::InputEventSource InputEventStage::getInputSourceFromParsedChannel(MidiReceiveChannel channel)
{
  const auto primary = m_options->getMIDIPrimaryReceiveChannel();
  const auto secondary = m_options->getMIDISplitReceiveChannel();
  const auto primaryMask = midiReceiveChannelMask(primary);
  const auto secondaryMask = midiReceiveChannelMask(secondary);
  // first check: did event qualify at all?
  const bool qualifiedForEvent = (primaryMask | secondaryMask) > 0;
  if(!qualifiedForEvent)
    return DSPInterface::InputEventSource::Invalid;
  // second check: obtain channel mask and evaluate common "singular" case
  const auto channelMask = midiReceiveChannelMask(channel);
  const bool qualifiedForCommon = secondary == MidiReceiveChannelSplit::Common;
  const bool qualifiedForPrimary = (channelMask & primaryMask) > 0;
  if(qualifiedForCommon)
    return qualifiedForPrimary ? DSPInterface::InputEventSource::External_Use_Split
                               : DSPInterface::InputEventSource::Invalid;

  // third checks: evaluate "separate" cases (both, primary, secondary)
  const bool qualifiedForSecondary = (channelMask & secondaryMask) > 0;
  const bool qualifiedForIdentity = qualifiedForPrimary && qualifiedForSecondary;
  if(qualifiedForIdentity)
    return DSPInterface::InputEventSource::External_Both;
  if(qualifiedForPrimary)
    return DSPInterface::InputEventSource::External_Primary;
  if(qualifiedForSecondary)
    return DSPInterface::InputEventSource::External_Secondary;
  // this line should never be reached
  return DSPInterface::InputEventSource::Invalid;
}

void InputEventStage::doSendAftertouchOut(float value)
{
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;

  auto atLSB = m_options->getAftertouchLSBCC();
  auto atMSB = m_options->getAftertouchMSBCC();

  if(atLSB.has_value() && atMSB.has_value())
  {
    doSendCCOut(CC_Range_14_Bit::encodeUnipolarMidiValue(value), atMSB.value(), atLSB.value(),
                HardwareSource::AFTERTOUCH);
  }
  else if(m_options->getAftertouchSetting() == AftertouchCC::ChannelPressure)
  {
    const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDIPrimarySendChannel());
    const auto mainC = static_cast<uint8_t>(mainChannel);
    const auto secChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDISplitSendChannel());
    const auto secC = static_cast<uint8_t>(secChannel);

    auto atStatusByte = static_cast<uint8_t>(0xD0);
    uint8_t valByte = CC_Range_7_Bit::encodeUnipolarMidiValue(value);  //msb

    if(!latchHWPosition<LatchMode::OnlyMSB>(HardwareSource::AFTERTOUCH, 0, valByte))
      return;

    if(mainChannel != -1 && m_options->shouldSendMidiOnPrimary(RoutingIndex::Aftertouch))
    {
      auto mainStatus = static_cast<uint8_t>(atStatusByte | mainC);
      m_midiOut({ mainStatus, valByte });
    }

    if(secChannel != -1 && isSplitDSP() && m_options->shouldSendMidiOnSplit(RoutingIndex::Aftertouch))
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

    if(!latchHWPosition<LatchMode::LSBAndMSB>(HardwareSource::AFTERTOUCH, lsb, msb))
      return;

    const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDIPrimarySendChannel());
    const auto secChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDISplitSendChannel());
    const auto mainC = static_cast<uint8_t>(mainChannel);
    const auto secC = static_cast<uint8_t>(secChannel);
    auto statusByte = static_cast<uint8_t>(0xE0);

    if(mainChannel != -1 && m_options->shouldSendMidiOnPrimary(RoutingIndex::Aftertouch))
    {
      auto mainStatus = static_cast<uint8_t>(statusByte | mainC);
      m_midiOut({ mainStatus, lsb, msb });
    }

    if(secChannel != -1 && isSplitDSP() && m_options->shouldSendMidiOnSplit(RoutingIndex::Aftertouch))
    {
      auto secStatus = static_cast<uint8_t>(statusByte | secC);
      m_midiOut({ secStatus, lsb, msb });
    }
  }
}

void InputEventStage::doSendBenderOut(float value)
{
  auto benderLSB = m_options->getBenderLSBCC();
  auto benderMSB = m_options->getBenderMSBCC();

  if(benderLSB.has_value() && benderMSB.has_value())
  {
    using CC_Range_14_Bit = Midi::clipped14BitCCRange;
    auto lsbCC = benderLSB.value();
    auto msbCC = benderMSB.value();
    doSendCCOut(CC_Range_14_Bit::encodeBipolarMidiValue(value), msbCC, lsbCC, HardwareSource::BENDER);
  }
  else if(m_options->getBenderSetting() != BenderCC::None)
  {
    using CC_Range_Bender = Midi::FullCCRange<Midi::Formats::_14_Bits_>;

    auto v = CC_Range_Bender::encodeBipolarMidiValue(value);
    auto lsb = static_cast<uint8_t>(v & 0x7F);
    auto msb = static_cast<uint8_t>((v >> 7) & 0x7F);

    if(!latchHWPosition<LatchMode::LSBAndMSB>(HardwareSource::BENDER, lsb, msb))
      return;

    const auto mainChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDIPrimarySendChannel());
    const auto secChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDISplitSendChannel());
    const auto mainC = static_cast<uint8_t>(mainChannel);
    const auto secC = static_cast<uint8_t>(secChannel);
    auto statusByte = static_cast<uint8_t>(0xE0);

    if(mainChannel != -1 && m_options->shouldSendMidiOnPrimary(RoutingIndex::Bender))
    {
      auto mainStatus = static_cast<uint8_t>(statusByte | mainC);
      m_midiOut({ mainStatus, lsb, msb });
    }

    if(secChannel != -1 && isSplitDSP() && m_options->shouldSendMidiOnSplit(RoutingIndex::Bender))
    {
      auto secStatus = static_cast<uint8_t>(statusByte | secC);
      m_midiOut({ secStatus, lsb, msb });
    }
  }
}

void InputEventStage::onUIHWSourceMessage(const nltools::msg::HWSourceChangedMessage &message, bool didBehaviourChange)
{
  auto hwID = InputEventStage::parameterIDToHWID(message.parameterId);

  if(hwID != HardwareSource::NONE)
  {
    auto cp = static_cast<float>(message.controlPosition);
    onHWChanged(hwID, cp, HWChangeSource::UI, false, false, didBehaviourChange);
  }
}

HardwareSource InputEventStage::parameterIDToHWID(int id)
{
  switch(id)
  {
    case C15::PID::Pedal_1:
      return HardwareSource::PEDAL1;
    case C15::PID::Pedal_2:
      return HardwareSource::PEDAL2;
    case C15::PID::Pedal_3:
      return HardwareSource::PEDAL3;
    case C15::PID::Pedal_4:
      return HardwareSource::PEDAL4;
    case C15::PID::Bender:
      return HardwareSource::BENDER;
    case C15::PID::Aftertouch:
      return HardwareSource::AFTERTOUCH;
    case C15::PID::Ribbon_1:
      return HardwareSource::RIBBON1;
    case C15::PID::Ribbon_2:
      return HardwareSource::RIBBON2;
    default:
      return HardwareSource::NONE;
  }
}

void InputEventStage::onHWChanged(HardwareSource hwID, float pos, HWChangeSource source, bool wasMIDIPrimary,
                                  bool wasMIDISplit, bool didBehaviourChange)
{
  auto sendToDSP = [&](auto source, auto hwID, auto wasPrim, auto wasSplit)
  {
    const auto routingIndex = static_cast<RoutingIndex>(hwID);

    switch(source)
    {
      case HWChangeSource::MIDI:
      {
        if(wasPrim)
          return m_options->shouldReceiveMidiOnPrimary(routingIndex);
        else if(wasSplit)
          return m_options->shouldReceiveMidiOnSplit(routingIndex);

        return true;
      }
      case HWChangeSource::TCD:
      case HWChangeSource::UI:
        return m_options->shouldAllowLocal(routingIndex);
      default:
        nltools_assertNotReached();
    }
  };

  if(sendToDSP(source, hwID, wasMIDIPrimary, wasMIDISplit))
  {
    m_dspHost->onHWChanged(hwID, pos, didBehaviourChange);
    m_localDisabledPositions[static_cast<unsigned int>(hwID)] = { pos, source };
    m_hwChangedCB();
  }
  else if(source != HWChangeSource::MIDI)
  {
    m_localDisabledPositions[static_cast<unsigned int>(hwID)] = { pos, source };
    m_hwChangedCB();
  }

  if(source != HWChangeSource::MIDI)
  {
    const auto isPedal = hwID >= HardwareSource::PEDAL1 && hwID <= HardwareSource::PEDAL4;
    if(isPedal && m_options->isSwitchingCC(hwID))
    {
      pos = pos >= 0.5f ? 1.0f : 0.0f;
    }

    sendHardwareChangeAsMidi(hwID, pos);
  }
}

float processMidiForHWSource(DSPInterface *dsp, HardwareSource id, uint8_t msb, uint8_t lsb)
{
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;
  auto midiVal = (static_cast<uint16_t>(msb) << 7) + lsb;

  if(dsp->getBehaviour(id) == C15::Properties::HW_Return_Behavior::Center)
    return CC_Range_14_Bit::decodeBipolarMidiValue(midiVal);
  else
    return CC_Range_14_Bit::decodeUnipolarMidiValue(midiVal);
}

HardwareSource ccToHWID(int cc, MidiRuntimeOptions *options)
{
  if(options && cc != -1)
  {
    const auto p1 = options->getCCFor<Midi::MSB::Ped1>();
    const auto p2 = options->getCCFor<Midi::MSB::Ped2>();
    const auto p3 = options->getCCFor<Midi::MSB::Ped3>();
    const auto p4 = options->getCCFor<Midi::MSB::Ped4>();
    const auto at = options->getCCFor<Midi::MSB::Aftertouch>();
    const auto bender = options->getCCFor<Midi::MSB::Bender>();
    const auto ribbon1 = options->getCCFor<Midi::MSB::Rib1>();
    const auto ribbon2 = options->getCCFor<Midi::MSB::Rib2>();

    if(p1 == cc)
    {
      return HardwareSource::PEDAL1;
    }
    else if(p2 == cc)
    {
      return HardwareSource::PEDAL2;
    }
    else if(p3 == cc)
    {
      return HardwareSource::PEDAL3;
    }
    else if(p4 == cc)
    {
      return HardwareSource::PEDAL4;
    }
    else if(at == cc)
    {
      return HardwareSource::AFTERTOUCH;
    }
    else if(bender == cc)
    {
      return HardwareSource::BENDER;
    }
    else if(ribbon1 == cc)
    {
      return HardwareSource::RIBBON1;
    }
    else if(ribbon2 == cc)
    {
      return HardwareSource::RIBBON2;
    }
  }
  return HardwareSource::NONE;
}

void InputEventStage::onMIDIHWChanged(MIDIDecoder *decoder)
{
  auto hwControlIDOrCC = decoder->getKeyOrControl();
  auto hwControlID = static_cast<HardwareSource>(hwControlIDOrCC);
  auto hwRes = decoder->getHWChangeStruct();

  if(hwRes.cases == MIDIDecoder::MidiHWChangeSpecialCases::CC)
  {
    if(ccIsMappedToChannelModeMessage(hwRes.receivedCC))
    {
      auto msbCCValue = hwRes.undecodedValueBytes[0];
      queueChannelModeMessage(hwRes.receivedCC, msbCCValue);
      return;
    }
  }

  if(hwControlID == HardwareSource::NONE)
  {
    switch(hwRes.cases)
    {
      case MIDIDecoder::MidiHWChangeSpecialCases::ChannelPitchbend:
        hwControlID = HardwareSource::AFTERTOUCH;
        break;
      case MIDIDecoder::MidiHWChangeSpecialCases::Aftertouch:
        hwControlID = HardwareSource::BENDER;
        break;
      default:
      case MIDIDecoder::MidiHWChangeSpecialCases::CC:
        hwControlID = ccToHWID(decoder->getHWChangeStruct().receivedCC, m_options);
        break;
    }
  }

  if(hwControlID == HardwareSource::NONE)
    return;

  const auto isSplit = m_dspHost->getType() == SoundType::Split;
  const auto isPrimaryChannel = decoder->getChannel() == m_options->getMIDIPrimaryReceiveChannel();

  const auto routingIndex = toRoutingIndex(hwControlID);
  const auto shouldReceiveOnPrim = m_options->shouldReceiveMidiOnPrimary(routingIndex);
  const auto shouldReceiveOnSplit = m_options->shouldReceiveMidiOnSplit(routingIndex);
  const auto isSplitChannel
      = MidiRuntimeOptions::normalToSplitChannel(decoder->getChannel()) == m_options->getMIDISplitReceiveChannel();
  const auto receiveOmni = m_options->getMIDIPrimaryReceiveChannel() == MidiReceiveChannel::Omni
      || m_options->getMIDISplitReceiveChannel() == MidiReceiveChannelSplit::Omni;

  const auto primaryAndAllowed = isPrimaryChannel && shouldReceiveOnPrim;
  const auto splitAndAllowed = isSplit && isSplitChannel && shouldReceiveOnSplit;
  const auto omniAndAllowed = receiveOmni && (shouldReceiveOnPrim || shouldReceiveOnSplit);

  if(primaryAndAllowed || splitAndAllowed || omniAndAllowed)
  {
    for(auto hw : sHardwareSources)
    {
      const auto mappedMSBCC = m_options->getMSBCCForHWID(hw);
      if(mappedMSBCC != -1 && mappedMSBCC == hwRes.receivedCC)
      {
        const auto msb = hwRes.undecodedValueBytes[0];
        const auto lsb = hwRes.undecodedValueBytes[1];
        float realVal = processMidiForHWSource(m_dspHost, hw, msb, lsb);
        m_dspHost->onHWChanged(hw, realVal, false);
        m_localDisabledPositions[static_cast<unsigned int>(hw)] = { realVal, HWChangeSource::MIDI };
        m_hwChangedCB();
      }
      else
      {
        if(hw == HardwareSource::BENDER && hwRes.cases == MIDIDecoder::MidiHWChangeSpecialCases::ChannelPitchbend)
        {
          if(m_options->getBenderSetting() == BenderCC::Pitchbend)
          {
            onHWChanged(HardwareSource::BENDER, decoder->getValue(), HWChangeSource::MIDI, isPrimaryChannel,
                        isSplitChannel, false);
          }
        }

        if(hw == HardwareSource::AFTERTOUCH)
        {
          if(hwRes.cases == MIDIDecoder::MidiHWChangeSpecialCases::Aftertouch)
          {
            if(m_options->getAftertouchSetting() == AftertouchCC::ChannelPressure)
            {
              onHWChanged(HardwareSource::AFTERTOUCH, decoder->getValue(), HWChangeSource::MIDI, isPrimaryChannel,
                          isSplitChannel, false);
            }
          }

          if(hwRes.cases == MIDIDecoder::MidiHWChangeSpecialCases::ChannelPitchbend)
          {
            auto pitchbendValue = decoder->getValue();

            if(m_options->getAftertouchSetting() == AftertouchCC::PitchbendUp)
            {
              pitchbendValue = std::max(0.0f, pitchbendValue);
              onHWChanged(HardwareSource::AFTERTOUCH, pitchbendValue, HWChangeSource::MIDI, isPrimaryChannel,
                          isSplitChannel, false);
            }
            else if(m_options->getAftertouchSetting() == AftertouchCC::PitchbendDown)
            {
              pitchbendValue = -std::min(0.0f, pitchbendValue);
              onHWChanged(HardwareSource::AFTERTOUCH, pitchbendValue, HWChangeSource::MIDI, isPrimaryChannel,
                          isSplitChannel, false);
            }
          }
        }
      }
    }
  }
}

constexpr uint16_t InputEventStage::midiReceiveChannelMask(const MidiReceiveChannel &_channel)
{
  return c_midiReceiveMaskTable[static_cast<uint8_t>(_channel)];
}

constexpr uint16_t InputEventStage::midiReceiveChannelMask(const MidiReceiveChannelSplit &_channel)
{
  return c_midiReceiveMaskTable[static_cast<uint8_t>(_channel)];
}

bool InputEventStage::getAndResetKeyBedStatus()
{
  return std::exchange(m_notifyKeyBedActionStatus, false);
}

void InputEventStage::setAndScheduleKeybedNotify()
{
  m_notifyKeyBedActionStatus = true;
  m_hwChangedCB();
}

bool InputEventStage::isSplitDSP() const
{
  return m_dspHost->getType() == SoundType::Split;
}

bool InputEventStage::ccIsMappedToChannelModeMessage(int cc)
{
  return MidiRuntimeOptions::isCCMappedToChannelModeMessage(cc);
}

void InputEventStage::queueChannelModeMessage(int cc, uint8_t msbCCvalue)
{
  m_channelModeMessageCB(MidiRuntimeOptions::createChannelModeMessageEnum(cc, msbCCvalue));
}

void InputEventStage::onMidiSettingsMessageWasReceived(const tMSG &msg, const tMSG &oldmsg)
{
  handlePressedNotesOnMidiSettingsChanged(msg, oldmsg);
  handleHWSourcesWhichGotTurnedOff(msg, oldmsg);
  handleHWSourcesWhichCCsChanged(msg, oldmsg);
}

void InputEventStage::handlePressedNotesOnMidiSettingsChanged(const tMSG &msg, const tMSG &oldConfig)
{

  constexpr auto Notes = static_cast<int>(RoutingIndex::Notes);
  constexpr auto SendPrim = static_cast<int>(RoutingAspect::SEND_PRIMARY);
  constexpr auto SendSplit = static_cast<int>(RoutingAspect::SEND_SPLIT);
  constexpr auto RecvPrim = static_cast<int>(RoutingAspect::RECEIVE_PRIMARY);
  constexpr auto RecvSplit = static_cast<int>(RoutingAspect::RECEIVE_SPLIT);
  constexpr auto Local = static_cast<int>(RoutingAspect::LOCAL);

  const auto &oldNotesData = oldConfig.routings[Notes];

  constexpr auto CCNum = static_cast<uint8_t>(MidiRuntimeOptions::MidiChannelModeMessageCCs::AllNotesOff);
  constexpr uint8_t CCModeChange = 0b10110000;

  const auto oldPrimSendState = oldNotesData[SendPrim];
  const auto oldSecSendState = oldNotesData[SendSplit];
  const auto oldReceivePrimState = oldNotesData[RecvPrim];
  const auto oldReceiveSplitState = oldNotesData[RecvSplit];
  const auto oldLocalState = oldNotesData[Local] && oldConfig.localEnable;

  const auto isSplit = m_dspHost->getType() == SoundType::Split;

  const auto isSendPrim = msg.routings[Notes][SendPrim];
  const auto isSendSplit = msg.routings[Notes][SendSplit];
  const auto isReceivePrim = msg.routings[Notes][RecvPrim];
  const auto isReceiveSplit = msg.routings[Notes][RecvSplit];
  const auto isLocalEnabled = msg.routings[Notes][Local] && msg.localEnable;

  const auto primSendNowDisabled = !isSendPrim && oldPrimSendState;
  const auto splitSendNowDisabled = !isSendSplit && oldSecSendState;
  const auto primReceiveNowDisabled = !isReceivePrim && oldReceivePrimState;
  const auto splitReceiveNowDisabled = !isReceiveSplit && oldReceiveSplitState;
  const auto localNowDisabled = !isLocalEnabled && oldLocalState;

  const auto didPrimSendChange = msg.sendChannel != oldConfig.sendChannel;
  const auto didSecSendChange = msg.sendSplitChannel != oldConfig.sendSplitChannel;
  const auto didPrimRecChange = msg.receiveChannel != oldConfig.receiveChannel;
  const auto didSecRecChange = msg.receiveSplitChannel != oldConfig.receiveSplitChannel;

  const auto primIsNowDisabled = !isSendPrim && oldPrimSendState;
  const auto splitIsNowDisabled = !isSendSplit && oldSecSendState;

  auto sendNotesOffOnChannel = [&](auto channel)
  {
    const auto iChannel = MidiRuntimeOptions::channelEnumToInt(channel);

    if(iChannel != -1)
    {
      m_midiOut({ static_cast<uint8_t>(CCModeChange | iChannel), CCNum, 0 });
    }
  };

  auto sendFadeAndMuteToDSP = [&]() { m_channelModeMessageCB(MidiChannelModeMessages::AllSoundOff); };

  const auto prim = m_options->getMIDIPrimarySendChannel();
  const auto sec = m_options->getMIDISplitSendChannel();

  bool didScheduleAllSoundsOff = false;

  if(primSendNowDisabled)
  {
    sendNotesOffOnChannel(prim);
  }
  else if(primReceiveNowDisabled || didPrimRecChange)
  {
    sendFadeAndMuteToDSP();
    didScheduleAllSoundsOff = true;
  }
  else if(didPrimSendChange)
  {
    sendNotesOffOnChannel(oldConfig.sendChannel);
  }
  else if(localNowDisabled)
  {
    sendFadeAndMuteToDSP();
    didScheduleAllSoundsOff = true;
  }

  if(isSplit)
  {
    if(splitSendNowDisabled)
    {
      sendNotesOffOnChannel(sec);
    }
    else if(splitReceiveNowDisabled || didSecRecChange)
    {
      if(!didScheduleAllSoundsOff)
        sendFadeAndMuteToDSP();
      didScheduleAllSoundsOff = true;
    }
    else if(didSecSendChange)
    {
      sendNotesOffOnChannel(oldConfig.sendSplitChannel);
    }
    else if(localNowDisabled)
    {
      if(!didScheduleAllSoundsOff)
        sendFadeAndMuteToDSP();
      didScheduleAllSoundsOff = true;
    }
  }
}

void InputEventStage::handleHWSourcesWhichGotTurnedOff(const tMSG &msg, const tMSG &oldmsg)
{
  auto to = [](auto x) { return static_cast<int>(x); };
  auto from = [](auto x) { return static_cast<tMSG::RoutingIndex>(x); };
  auto idToID = [](tMSG::RoutingIndex x)
  {
    switch(x)
    {
      case RoutingIndex::Pedal1:
        return HardwareSource::PEDAL1;
      case RoutingIndex::Pedal2:
        return HardwareSource::PEDAL2;
      case RoutingIndex::Pedal3:
        return HardwareSource::PEDAL3;
      case RoutingIndex::Pedal4:
        return HardwareSource::PEDAL4;
      case RoutingIndex::Bender:
        return HardwareSource::BENDER;
      case RoutingIndex::Aftertouch:
        return HardwareSource::AFTERTOUCH;
      case RoutingIndex::Ribbon1:
        return HardwareSource::RIBBON1;
      case RoutingIndex::Ribbon2:
        return HardwareSource::RIBBON2;
      default:
      case RoutingIndex::ProgramChange:
      case RoutingIndex::Notes:
      case RoutingIndex::LENGTH:
        return HardwareSource::NONE;
    }
  };

  const auto primSendChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDIPrimarySendChannel());
  const auto oldPrimSendChannel = MidiRuntimeOptions::channelEnumToInt(oldmsg.sendChannel);
  const auto splitSendChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDISplitSendChannel());
  const auto oldSplitSendChannel = MidiRuntimeOptions::channelEnumToInt(oldmsg.sendSplitChannel);
  const auto primReceiveChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDIPrimaryReceiveChannel());
  const auto oldPrimReceiveChannel = MidiRuntimeOptions::channelEnumToInt(oldmsg.receiveChannel);
  const auto splitReceiveChannel = MidiRuntimeOptions::channelEnumToInt(m_options->getMIDISplitReceiveChannel());
  const auto oldSplitReceiveChannel = MidiRuntimeOptions::channelEnumToInt(oldmsg.receiveSplitChannel);

  const auto didPrimSendChannelChange = primSendChannel != oldPrimSendChannel;
  const auto didSplitSendChannelChange = splitSendChannel != oldSplitSendChannel;
  const auto didPrimReceiveChannelChange = primReceiveChannel != oldPrimReceiveChannel;
  const auto didSplitReceiveChannelChange = splitReceiveChannel != oldSplitReceiveChannel;

  int hwIdx = 0;
  for(auto &hw : msg.routings)
  {

    if(from(hwIdx) != tMSG::RoutingIndex::Notes && from(hwIdx) != tMSG::RoutingIndex::ProgramChange)
    {
      const auto didPrimReceiveChange = hw[to(tMSG::RoutingAspect::RECEIVE_PRIMARY)]
          != oldmsg.routings[hwIdx][to(tMSG::RoutingAspect::RECEIVE_PRIMARY)];
      const auto didSplitReceiveChange = hw[to(tMSG::RoutingAspect::RECEIVE_SPLIT)]
          != oldmsg.routings[hwIdx][to(tMSG::RoutingAspect::RECEIVE_SPLIT)];
      const auto didPrimSendChange
          = hw[to(tMSG::RoutingAspect::SEND_PRIMARY)] != oldmsg.routings[hwIdx][to(tMSG::RoutingAspect::SEND_PRIMARY)];
      const auto didSplitSendChange
          = hw[to(tMSG::RoutingAspect::SEND_SPLIT)] != oldmsg.routings[hwIdx][to(tMSG::RoutingAspect::SEND_SPLIT)];
      const auto didLocalChange
          = hw[to(tMSG::RoutingAspect::LOCAL)] != oldmsg.routings[hwIdx][to(tMSG::RoutingAspect::LOCAL)];
      const auto hwID = idToID(from(hwIdx));

      if(m_dspHost->getBehaviour(hwID) != C15::Properties::HW_Return_Behavior::Stay)
      {
        auto value = m_dspHost->getReturnValueFor(hwID);
        if(didPrimSendChange)
          sendHardwareChangeAsMidiOnExplicitChannel(hwID, value, primSendChannel);
        if(didSplitSendChange && m_dspHost->getType() == SoundType::Split)
          sendHardwareChangeAsMidiOnExplicitChannel(hwID, value, splitSendChannel);
        if(didPrimReceiveChange)
          m_dspHost->resetReturningHWSource(hwID);
        if(didSplitReceiveChange && m_dspHost->getType() == SoundType::Split)
          m_dspHost->resetReturningHWSource(hwID);
        if(didLocalChange)
        {
          m_dspHost->resetReturningHWSource(hwID);
          sendHardwareChangeAsMidiOnExplicitChannel(hwID, value, primSendChannel);
          if(m_dspHost->getType() == SoundType::Split)
            sendHardwareChangeAsMidiOnExplicitChannel(hwID, value, splitSendChannel);
        }

        if(didPrimSendChannelChange)
          sendHardwareChangeAsMidiOnExplicitChannel(hwID, value, oldPrimSendChannel);

        if(didSplitSendChannelChange && m_dspHost->getType() == SoundType::Split)
          sendHardwareChangeAsMidiOnExplicitChannel(hwID, value, oldSplitSendChannel);

        if(didPrimReceiveChannelChange)
          m_dspHost->resetReturningHWSource(hwID);
        if(didSplitReceiveChannelChange && m_dspHost->getType() == SoundType::Split)
          m_dspHost->resetReturningHWSource(hwID);
      }
    }
    hwIdx++;
  }
}

void InputEventStage::sendHardwareChangeAsMidiOnExplicitChannel(HardwareSource id, float value, int channel)
{
  switch(id)
  {
    case HardwareSource::PEDAL1:
      sendCCOutOnExplicitChannel(HardwareSource::PEDAL1, value, m_options->getCCFor<Midi::MSB::Ped1>(),
                                 m_options->getCCFor<Midi::LSB::Ped1>(), channel);
      break;

    case HardwareSource::PEDAL2:
      sendCCOutOnExplicitChannel(HardwareSource::PEDAL2, value, m_options->getCCFor<Midi::MSB::Ped2>(),
                                 m_options->getCCFor<Midi::LSB::Ped2>(), channel);
      break;

    case HardwareSource::PEDAL3:
      sendCCOutOnExplicitChannel(HardwareSource::PEDAL3, value, m_options->getCCFor<Midi::MSB::Ped3>(),
                                 m_options->getCCFor<Midi::LSB::Ped3>(), channel);
      break;

    case HardwareSource::PEDAL4:
      sendCCOutOnExplicitChannel(HardwareSource::PEDAL4, value, m_options->getCCFor<Midi::MSB::Ped4>(),
                                 m_options->getCCFor<Midi::LSB::Ped4>(), channel);
      break;

    case HardwareSource::BENDER:
      doSendBenderOutOnExplicitChannel(value, channel);
      break;

    case HardwareSource::AFTERTOUCH:
      doSendAftertouchOutOnExplicitChannel(value, channel);
      break;

    case HardwareSource::RIBBON1:
      sendCCOutOnExplicitChannel(HardwareSource::RIBBON1, value, m_options->getCCFor<Midi::MSB::Rib1>(),
                                 m_options->getCCFor<Midi::LSB::Rib1>(), channel);
      break;

    case HardwareSource::RIBBON2:
      sendCCOutOnExplicitChannel(HardwareSource::RIBBON2, value, m_options->getCCFor<Midi::MSB::Rib2>(),
                                 m_options->getCCFor<Midi::LSB::Rib2>(), channel);
      break;

    default:
      break;
  }
}

void InputEventStage::sendCCOutOnExplicitChannel(HardwareSource hwID, float value, int msbCC, int lsbCC, int channel)
{
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;

  if(m_dspHost->getBehaviour(hwID) == C15::Properties::HW_Return_Behavior::Center)
    doSendCCOutOnExplicitChannel(CC_Range_14_Bit::encodeBipolarMidiValue(value), msbCC, lsbCC, hwID, channel);
  else
    doSendCCOutOnExplicitChannel(CC_Range_14_Bit::encodeUnipolarMidiValue(value), msbCC, lsbCC, hwID, channel);
}

void InputEventStage::doSendCCOutOnExplicitChannel(uint16_t value, int msbCC, int lsbCC, HardwareSource hwID,
                                                   int channel)
{

  auto statusByte = static_cast<uint8_t>(0xB0);
  auto lsbValByte = static_cast<uint8_t>(value & 0x7F);
  auto msbValByte = static_cast<uint8_t>(value >> 7 & 0x7F);

  if(!latchHWPosition<LatchMode::Option>(hwID, lsbValByte, msbValByte))
    return;

  if(channel != -1)
  {
    const auto mainC = static_cast<uint8_t>(channel);
    auto mainStatus = static_cast<uint8_t>(statusByte | mainC);

    if(lsbCC != -1 && m_options->is14BitSupportEnabled())
    {
      m_midiOut({ mainStatus, static_cast<uint8_t>(lsbCC), lsbValByte });
    }

    if(msbCC != -1)
    {
      m_midiOut({ mainStatus, static_cast<uint8_t>(msbCC), msbValByte });
    }
  }
}

void InputEventStage::doSendBenderOutOnExplicitChannel(float value, int channel)
{
  auto benderLSB = m_options->getBenderLSBCC();
  auto benderMSB = m_options->getBenderMSBCC();

  if(benderLSB.has_value() && benderMSB.has_value())
  {
    using CC_Range_14_Bit = Midi::clipped14BitCCRange;
    auto lsbCC = benderLSB.value();
    auto msbCC = benderMSB.value();
    doSendCCOutOnExplicitChannel(CC_Range_14_Bit::encodeBipolarMidiValue(value), msbCC, lsbCC, HardwareSource::BENDER,
                                 channel);
  }
  else if(m_options->getBenderSetting() != BenderCC::None)
  {
    using CC_Range_Bender = Midi::FullCCRange<Midi::Formats::_14_Bits_>;

    auto v = CC_Range_Bender::encodeBipolarMidiValue(value);
    auto lsb = static_cast<uint8_t>(v & 0x7F);
    auto msb = static_cast<uint8_t>((v >> 7) & 0x7F);

    if(!latchHWPosition<LatchMode::LSBAndMSB>(HardwareSource::BENDER, lsb, msb))
      return;

    const auto mainC = static_cast<uint8_t>(channel);
    auto statusByte = static_cast<uint8_t>(0xE0);

    if(channel != -1)
    {
      auto mainStatus = static_cast<uint8_t>(statusByte | mainC);
      m_midiOut({ mainStatus, lsb, msb });
    }
  }
}

void InputEventStage::doSendAftertouchOutOnExplicitChannel(float value, int channel)
{
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;

  auto atLSB = m_options->getAftertouchLSBCC();
  auto atMSB = m_options->getAftertouchMSBCC();

  if(atLSB.has_value() && atMSB.has_value())
  {
    doSendCCOutOnExplicitChannel(CC_Range_14_Bit::encodeUnipolarMidiValue(value), atMSB.value(), atLSB.value(),
                                 HardwareSource::AFTERTOUCH, channel);
  }
  else if(m_options->getAftertouchSetting() == AftertouchCC::ChannelPressure)
  {
    const auto mainC = static_cast<uint8_t>(channel);

    auto atStatusByte = static_cast<uint8_t>(0xD0);
    uint8_t valByte = CC_Range_7_Bit::encodeUnipolarMidiValue(value);  //msb

    if(!latchHWPosition<LatchMode::OnlyMSB>(HardwareSource::AFTERTOUCH, 0, valByte))
      return;

    if(channel != -1)
    {
      auto mainStatus = static_cast<uint8_t>(atStatusByte | mainC);
      m_midiOut({ mainStatus, valByte });
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

    if(!latchHWPosition<LatchMode::LSBAndMSB>(HardwareSource::AFTERTOUCH, lsb, msb))
      return;

    const auto mainC = static_cast<uint8_t>(channel);
    auto statusByte = static_cast<uint8_t>(0xE0);

    if(channel != -1)
    {
      auto mainStatus = static_cast<uint8_t>(statusByte | mainC);
      m_midiOut({ mainStatus, lsb, msb });
    }
  }
}

void InputEventStage::handleHWSourcesWhichCCsChanged(const tMSG &message, const tMSG &snapshot)
{
  const auto didPedal1Change = message.pedal1cc != snapshot.pedal1cc;
  const auto didPedal2Change = message.pedal2cc != snapshot.pedal2cc;
  const auto didPedal3Change = message.pedal3cc != snapshot.pedal3cc;
  const auto didPedal4Change = message.pedal4cc != snapshot.pedal4cc;
  const auto didRibbon1Change = message.ribbon1cc != snapshot.ribbon1cc;
  const auto didRibbon2Change = message.ribbon2cc != snapshot.ribbon2cc;
  const auto didAftertouchChange = message.aftertouchcc != snapshot.aftertouchcc;
  const auto didBenderChange = message.bendercc != snapshot.bendercc;

  const auto pedal1Returning
      = m_dspHost->getBehaviour(HardwareSource::PEDAL1) != C15::Properties::HW_Return_Behavior::Stay;
  const auto pedal2Returning
      = m_dspHost->getBehaviour(HardwareSource::PEDAL2) != C15::Properties::HW_Return_Behavior::Stay;
  const auto pedal3Returning
      = m_dspHost->getBehaviour(HardwareSource::PEDAL3) != C15::Properties::HW_Return_Behavior::Stay;
  const auto pedal4Returning
      = m_dspHost->getBehaviour(HardwareSource::PEDAL4) != C15::Properties::HW_Return_Behavior::Stay;
  const auto ribbon1Returning
      = m_dspHost->getBehaviour(HardwareSource::RIBBON1) != C15::Properties::HW_Return_Behavior::Stay;
  const auto ribbon2Returning
      = m_dspHost->getBehaviour(HardwareSource::RIBBON2) != C15::Properties::HW_Return_Behavior::Stay;
  const auto aftertouchReturning
      = m_dspHost->getBehaviour(HardwareSource::AFTERTOUCH) != C15::Properties::HW_Return_Behavior::Stay;
  const auto benderReturning
      = m_dspHost->getBehaviour(HardwareSource::BENDER) != C15::Properties::HW_Return_Behavior::Stay;

  if(didPedal1Change && pedal1Returning)
  {
    auto oldmsb = MidiRuntimeOptions::decodeEnumMSB(snapshot.pedal1cc);
    auto oldlsb = MidiRuntimeOptions::decodeEnumLSB(snapshot.pedal1cc);
    doSendCCOut(0, oldmsb.value_or(-1), oldlsb.value_or(-1), HardwareSource::PEDAL1);
  }
  if(didPedal2Change && pedal2Returning)
  {
    auto oldmsb = MidiRuntimeOptions::decodeEnumMSB(snapshot.pedal2cc);
    auto oldlsb = MidiRuntimeOptions::decodeEnumLSB(snapshot.pedal2cc);
    doSendCCOut(0, oldmsb.value_or(-1), oldlsb.value_or(-1), HardwareSource::PEDAL2);
  }
  if(didPedal3Change && pedal3Returning)
  {
    auto oldmsb = MidiRuntimeOptions::decodeEnumMSB(snapshot.pedal3cc);
    auto oldlsb = MidiRuntimeOptions::decodeEnumLSB(snapshot.pedal3cc);
    doSendCCOut(0, oldmsb.value_or(-1), oldlsb.value_or(-1), HardwareSource::PEDAL3);
  }
  if(didPedal4Change && pedal4Returning)
  {
    auto oldmsb = MidiRuntimeOptions::decodeEnumMSB(snapshot.pedal4cc);
    auto oldlsb = MidiRuntimeOptions::decodeEnumLSB(snapshot.pedal4cc);
    doSendCCOut(0, oldmsb.value_or(-1), oldlsb.value_or(-1), HardwareSource::PEDAL4);
  }
  if(didRibbon1Change && ribbon1Returning)
  {
    auto oldmsb = MidiRuntimeOptions::decodeEnumMSB(snapshot.ribbon1cc);
    auto oldlsb = MidiRuntimeOptions::decodeEnumLSB(snapshot.ribbon1cc);
    doSendCCOut(0, oldmsb.value_or(-1), oldlsb.value_or(-1), HardwareSource::RIBBON1);
  }
  if(didRibbon2Change && ribbon2Returning)
  {
    auto oldmsb = MidiRuntimeOptions::decodeEnumMSB(snapshot.ribbon2cc);
    auto oldlsb = MidiRuntimeOptions::decodeEnumLSB(snapshot.ribbon2cc);
    doSendCCOut(0, oldmsb.value_or(-1), oldlsb.value_or(-1), HardwareSource::RIBBON2);
  }
  if(didBenderChange && benderReturning)
  {
    auto oldmsb = MidiRuntimeOptions::decodeEnumMSB(snapshot.bendercc);
    auto oldlsb = MidiRuntimeOptions::decodeEnumLSB(snapshot.bendercc);
    doSendCCOut(0, oldmsb.value_or(-1), oldlsb.value_or(-1), HardwareSource::BENDER);
  }
  if(didAftertouchChange && aftertouchReturning)
  {
    auto oldmsb = MidiRuntimeOptions::decodeEnumMSB(snapshot.aftertouchcc);
    auto oldlsb = MidiRuntimeOptions::decodeEnumLSB(snapshot.aftertouchcc);
    doSendCCOut(0, oldmsb.value_or(-1), oldlsb.value_or(-1), HardwareSource::AFTERTOUCH);
  }
}

InputEventStage::RoutingIndex InputEventStage::toRoutingIndex(HardwareSource source)
{
  switch(source)
  {
    case HardwareSource::PEDAL1:
      return RoutingIndex::Pedal1;
    case HardwareSource::PEDAL2:
      return RoutingIndex::Pedal2;
    case HardwareSource::PEDAL3:
      return RoutingIndex::Pedal3;
    case HardwareSource::PEDAL4:
      return RoutingIndex::Pedal4;
    case HardwareSource::BENDER:
      return RoutingIndex::Bender;
    case HardwareSource::AFTERTOUCH:
      return RoutingIndex::Aftertouch;
    case HardwareSource::RIBBON1:
      return RoutingIndex::Ribbon1;
    case HardwareSource::RIBBON2:
      return RoutingIndex::Ribbon2;
    default:
    case HardwareSource::NONE:
      return InputEventStage::RoutingIndex::LENGTH;
      break;
  }
}

float InputEventStage::getHWSourcePositionIfLocalDisabled(HardwareSource hwid) const
{
  if(hwid != HardwareSource::NONE)
    return std::get<0>(m_localDisabledPositions[static_cast<unsigned int>(hwid)]);

  nltools::Log::error(__PRETTY_FUNCTION__, "got HardwareSource:", toString(hwid));
  nltools_assertAlways(false);
}

HWChangeSource InputEventStage::getHWSourcePositionSource(HardwareSource hwid) const
{
  if(hwid != HardwareSource::NONE)
    return std::get<1>(m_localDisabledPositions[static_cast<unsigned int>(hwid)]);

  nltools::Log::error(__PRETTY_FUNCTION__, "got HardwareSource:", toString(hwid));
  nltools_assertAlways(false);
}
