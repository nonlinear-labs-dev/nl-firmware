#include "MIDIDecoder.h"
#include <nltools/logging/Log.h>
#include <synth/c15-audio-engine/dsp_host_dual.h>

DecoderEventType statusToType(uint8_t status)
{
  auto statusWithoutChannel = (status & 0b11110000);
  switch(statusWithoutChannel)
  {
    case 144:
      return DecoderEventType::KeyDown;
    case 128:
      return DecoderEventType::KeyUp;
    case 176:
      return DecoderEventType::HardwareChange;
    default:
      nltools::Log::warning("Unkown Status Byte:", (int) status);
      return DecoderEventType::UNKNOWN;
  }
}

enum class MIDIEventTypes : int
{
  Note_Off = 0,
  Note_On = 1,
  CC = 3,
  Mono_Aftertouch = 5,
  Bender = 6
};

bool MIDIDecoder::decode(const MidiEvent& event)
{
  const uint32_t type = (event.raw[0] & 127) >> 4;
  const auto _data0 = event.raw[1];
  const auto _data1 = event.raw[2];

  switch((MIDIEventTypes) type)
  {
    case MIDIEventTypes::Note_Off:
    {
      const uint16_t fullResVel = (_data1 << 7) + std::exchange(m_velocityLSB, 0);
      value = CC_Range_Vel::decodeUnipolarMidiValue(fullResVel);
      keyOrControl = _data0;
      m_type = DecoderEventType::KeyUp;
    }
    break;
    case MIDIEventTypes::Note_On:
    {
      const uint16_t fullResVel = (_data1 << 7) + std::exchange(m_velocityLSB, 0);
      value = CC_Range_Vel::decodeUnipolarMidiValue(fullResVel);
      keyOrControl = _data0;
      if(CC_Range_Vel::isValidNoteOnVelocity(fullResVel))
        m_type = DecoderEventType::KeyDown;
      else
        m_type = DecoderEventType::KeyUp;
    }
    break;
    case MIDIEventTypes::CC:
      switch(_data0)
      {
        case Midi::getMSB::getCC<Midi::MSB::Ped1>():
          processMidiForHWSource(0, _data1);
          m_type = DecoderEventType::HardwareChange;
          break;

        case Midi::getMSB::getCC<Midi::MSB::Ped2>():
          processMidiForHWSource(1, _data1);
          m_type = DecoderEventType::HardwareChange;
          break;

        case Midi::getMSB::getCC<Midi::MSB::Ped3>():
          processMidiForHWSource(2, _data1);
          m_type = DecoderEventType::HardwareChange;
          break;

        case Midi::getMSB::getCC<Midi::MSB::Ped4>():
          processMidiForHWSource(3, _data1);
          m_type = DecoderEventType::HardwareChange;
          break;

        case Midi::getMSB::getCC<Midi::MSB::Rib1>():
          processMidiForHWSource(6, _data1);
          m_type = DecoderEventType::HardwareChange;
          break;

        case Midi::getMSB::getCC<Midi::MSB::Rib2>():
          processMidiForHWSource(7, _data1);
          m_type = DecoderEventType::HardwareChange;
          break;

        case Midi::getLSB::getCC<Midi::LSB::Ped1>():
          m_hwSourcesMidiLSB[0] = _data1 & 0x7F;
          break;

        case Midi::getLSB::getCC<Midi::LSB::Ped2>():
          m_hwSourcesMidiLSB[1] = _data1 & 0x7F;
          break;

        case Midi::getLSB::getCC<Midi::LSB::Ped3>():
          m_hwSourcesMidiLSB[2] = _data1 & 0x7F;
          break;

        case Midi::getLSB::getCC<Midi::LSB::Ped4>():
          m_hwSourcesMidiLSB[3] = _data1 & 0x7F;
          break;

        case Midi::getLSB::getCC<Midi::LSB::Rib1>():
          m_hwSourcesMidiLSB[6] = _data1 & 0x7F;
          break;

        case Midi::getLSB::getCC<Midi::LSB::Rib2>():
          m_hwSourcesMidiLSB[7] = _data1 & 0x7F;
          break;

        case Midi::getLSB::getCC<Midi::LSB::Vel>():
          m_velocityLSB = _data1 & 0x7F;
          break;

        default:
          break;
      }
      break;
    case MIDIEventTypes::Mono_Aftertouch:
      m_hwSourcesMidiLSB[5] = 0;
      keyOrControl = 5;
      value = CC_Range_7_Bit::decodeUnipolarMidiValue(_data0);
      m_type = DecoderEventType::HardwareChange;
      break;

    case MIDIEventTypes::Bender:
      m_hwSourcesMidiLSB[4] = _data0 & 0x7F;
      keyOrControl = 4;
      value = CC_Range_Bender::decodeBipolarMidiValue((_data1 << 7) + std::exchange(m_hwSourcesMidiLSB[4], 0));
      m_type = DecoderEventType::HardwareChange;
      break;

    default:
      break;
  }
  return m_type != DecoderEventType::UNKNOWN;
}

DecoderEventType MIDIDecoder::getEventType() const
{
  return m_type;
}

int MIDIDecoder::getKeyOrControl() const
{
  return keyOrControl;
}

float MIDIDecoder::getValue() const
{
  return value;
}

void MIDIDecoder::processMidiForHWSource(int id, uint32_t _data)
{
  if(m_dsp->getBehaviour(id) == C15::Properties::HW_Return_Behavior::Center)
    processBipolarMidiController<CC_Range_14_Bit>(_data, id);
  else
    processUnipolarMidiController<CC_Range_14_Bit>(_data, id);
}

template <typename Range> void MIDIDecoder::processBipolarMidiController(const uint32_t dataByte, int id)
{
  auto midiVal = (dataByte << 7) + std::exchange(m_hwSourcesMidiLSB[id], 0);
  value = Range::decodeBipolarMidiValue(midiVal);
}

template <typename Range> void MIDIDecoder::processUnipolarMidiController(const uint32_t dataByte, int id)
{
  auto midiVal = (dataByte << 7) + std::exchange(m_hwSourcesMidiLSB[id], 0);
  value = Range::decodeUnipolarMidiValue(midiVal);
}
