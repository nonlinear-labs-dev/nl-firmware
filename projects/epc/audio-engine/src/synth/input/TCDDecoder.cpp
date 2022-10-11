#include <synth/C15Synth.h>
#include "TCDDecoder.h"
#include <playcontroller/playcontroller-defs.h>

TCDDecoder::TCDDecoder(DSPInterface *dsp, MidiRuntimeOptions *options, KeyShift *keyShift)
    : m_dsp { dsp }
    , m_keyShift { keyShift }
    , m_options { options }
{
  reset();
}

HardwareSource toHWIDFromChannelAndSetting(const uint32_t channel, MidiRuntimeOptions *m_options)
{
  auto hwid = static_cast<HardwareSource>(channel);

  if(hwid == HardwareSource::RIBBON1 && m_options->isSecondSetOfRibbonsIsSelected())
    hwid = HardwareSource::RIBBON3;
  else if(hwid == HardwareSource::RIBBON2 && m_options->isSecondSetOfRibbonsIsSelected())
    hwid = HardwareSource::RIBBON4;

  return hwid;
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
    if(channel >= HW_SOURCE_ID_PEDAL_1 && channel <= HW_SOURCE_ID_RIBBON_2)
    {
      uint32_t arg = _data1 + (_data0 << 7);
      m_value = static_cast<float>(arg) * c_norm_hw;  // HW src normalization by 1 / 16000

      const auto hwid = toHWIDFromChannelAndSetting(channel, m_options);
      const auto behaviour = m_dsp->getBehaviour(hwid);

      if(behaviour == C15::Properties::HW_Return_Behavior::Center)
      {
        m_value = (2.0f * m_value) - 1.0f;
      }

      m_hwSource = hwid;
      m_type = DecoderEventType::HardwareChange;

      if constexpr(LOG_MIDI_TCD)
      {
        nltools::Log::warning("Got HW-Src:", toString(m_hwSource), "m_value:", m_value, "behaviour",
                              static_cast<int>(behaviour), "raw data0", (short) _data0, "raw data1", (short) _data1);
      }
    }
    else if(_status == AE_PROTOCOL_CMD)  // PlayController Command
    {
      switch(_data1)
      {
        case AE_PROTOCOL_CMD_TONE_OFF:  // developer: test tone off
          m_dsp->onSettingToneToggle(1);
          break;
        case AE_PROTOCOL_CMD_TONE_ONLY:  // developer: test tone only (no synth)
          m_dsp->onSettingToneToggle(2);
          break;
        case AE_PROTOCOL_CMD_TONE_ON:  // developer: test tone on (and synth)
          m_dsp->onSettingToneToggle(3);
          break;
        case AE_PROTOCOL_CMD_DEFAULT_SOUND:  // developer: default sound (single initial, out mix a: 100%)
          m_dsp->onSettingInitialSinglePreset();
          break;
        case AE_PROTOCOL_CMD_POLL_DATA_START:  // hw poll start
          m_type = DecoderEventType::PollStart;
          break;
        case AE_PROTOCOL_CMD_POLL_DATA_STOP:  // hw poll stop
          m_type = DecoderEventType::PollStop;
          break;
      }
    }
    else if(_status == AE_PROTOCOL_KEY_POS)  //Key Pos
    {
      m_key = _data1;
    }
    else if(_status == AE_PROTOCOL_KEY_DOWN)  //Key Down
    {
      uint32_t arg = _data1 + (_data0 << 7);
      m_key = m_keyShift->keyDown(m_key);

      if((m_key >= C15::Config::virtual_key_from) && (m_key <= C15::Config::virtual_key_to))
      {
        m_value = static_cast<float>(arg) * c_norm_vel;  // VEL normalization by 1 / 16383
        m_type = DecoderEventType::KeyDown;
      }
    }
    else if(_status == AE_PROTOCOL_KEY_UP)  //Key Up
    {
      uint32_t arg = _data1 + (_data0 << 7);
      m_key = m_keyShift->keyUp(m_key);

      if((m_key >= C15::Config::virtual_key_from) && (m_key <= C15::Config::virtual_key_to))
      {
        m_value = static_cast<float>(arg) * c_norm_vel;  // VEL normalization by 1 / 16383
        m_type = DecoderEventType::KeyUp;
      }
    }
  }
  return m_type != DecoderEventType::UNKNOWN;
}

DecoderEventType TCDDecoder::getEventType() const
{
  return m_type;
}

int TCDDecoder::getKey() const
{
  return m_key;
}

[[nodiscard]] HardwareSource TCDDecoder::getHardwareSource() const
{
  return m_hwSource;
}

float TCDDecoder::getValue() const
{
  return m_value;
}

void TCDDecoder::reset()
{
  m_value = 0;
  m_type = DecoderEventType::UNKNOWN;
  m_key = INVALID_KEY;
  m_hwSource = HardwareSource::NONE;
}
