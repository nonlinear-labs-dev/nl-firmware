#include <synth/C15Synth.h>
#include "TCDDecoder.h"

TCDDecoder::TCDDecoder(DSPInterface *dsp, MidiRuntimeOptions *options)
    : m_dsp { dsp }
    , m_options { options }
{
}

bool TCDDecoder::decode(const MidiEvent &event)
{
  const auto _status = event.raw[0];
  const auto _data0 = event.raw[1];
  const auto _data1 = event.raw[2];
  const uint32_t channel = _status & 0b00001111;
  const uint32_t st = (_status & 0b01111111) >> 4;

  if(st == 6)
  {
    if(channel >= 0 && channel <= 7)
    {
      uint32_t arg = _data1 + (_data0 << 7);
      value = static_cast<float>(arg);
      keyOrController = channel;
      m_type = DecoderEventType::HardwareChange;
    }
    else if(channel == 13)
    {  //Key Pos
      keyOrController = _data1;
    }
    else if(channel == 14)
    {  //Key Down
      uint32_t arg = _data1 + (_data0 << 7);
      //keyOrController = m_shiftable_keys.keyDown(keyOrController); TODO fix
      if((keyOrController >= C15::Config::virtual_key_from) && (keyOrController <= C15::Config::virtual_key_to))
      {
        value = static_cast<float>(arg) * c_norm_vel;
        m_type = DecoderEventType::KeyDown;
        /*        uint8_t highResolutionVelocityStatusByte = static_cast<uint8_t>(0xB0);
    //        uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(vel);
//        uint8_t lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
//        out({ highResolutionVelocityStatusByte, 88, lsbVelByte });
//
//        uint8_t statusByte = static_cast<uint8_t>(0x90);
//        uint8_t keyByte = static_cast<uint8_t>(keyOrController) & 0x7F;
//        uint8_t msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
 //        out({ statusByte, keyByte, msbVelByte });
  */
      }
    }
    else if(channel == 15)
    {
      uint32_t arg = _data1 + (_data0 << 7);
      //keyOrController = m_shifteable_keys.keyUp(keyOrController); TODO Fix
      if((keyOrController >= C15::Config::virtual_key_from) && (keyOrController <= C15::Config::virtual_key_to))
      {
        value = static_cast<float>(arg) * c_norm_vel;
        m_type = DecoderEventType::KeyUp;
        /*        keyUp(vel);
        //
        //        uint8_t highResolutionVelocityStatusByte = static_cast<uint8_t>(0xB0);
        //        uint16_t fullResolutionValue = CC_Range_Vel::encodeUnipolarMidiValue(vel);
        //        uint8_t lsbVelByte = static_cast<uint8_t>(fullResolutionValue & 0x7F);
        //        out({ highResolutionVelocityStatusByte, 88, lsbVelByte });
        //
        //        uint8_t statusByte = static_cast<uint8_t>(0x80);
        //        uint8_t keyByte = static_cast<uint8_t>(keyOrController) & 0x7F;
        //        uint8_t msbVelByte = static_cast<uint8_t>(fullResolutionValue >> 7);
        //        out({ statusByte, keyByte, msbVelByte });
         */
      }
    }
  }
  return m_type != DecoderEventType::UNKNOWN;
}

DecoderEventType TCDDecoder::getEventType() const
{
  return m_type;
}

int TCDDecoder::getKeyOrController() const
{
  return keyOrController;
}

float TCDDecoder::getValue() const
{
  return value;
}
