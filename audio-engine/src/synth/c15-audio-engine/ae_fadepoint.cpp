#include "ae_fadepoint.h"

/******************************************************************************/
/** @file       ae_fadepoint.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      fadepoint generator
    @todo
*******************************************************************************/

#include <cmath>

ae_fade_table::ae_fade_table()
{
}

void ae_fade_table::init(const float _samplerate)
{
  float fade_time = 1e-3f * C15::Config::fade_time_ms * _samplerate, fade_dx = 1.0f / (fade_time - 1.0f);

  m_table_index = 0;
  m_trigger = false;
  m_trigger_index = static_cast<uint32_t>(fade_time);
  m_data.resize(2 * m_trigger_index);
  m_event = FadeEvent::None;
  m_value = 0.0f;

  uint32_t i;
  for(i = 0; i < m_trigger_index - 1; i++)
  {
    float x = std::cos(1.5707963f * static_cast<float>(i) * fade_dx);
    m_data[i] = x * x;
  }
  m_data[i] = 0.0f;
  for(i = 0; i < m_trigger_index; i++)
  {
    m_data[m_trigger_index + i] = m_data[m_trigger_index - i - 1];
  }
}

bool ae_fade_table::enable(const FadeEvent _event, const uint32_t _in_or_out)
{
  if(m_event == FadeEvent::None)
  {
    m_event = _event;
    m_trigger = true;
    m_table_offset = _in_or_out * m_trigger_index;
    m_table_index = 0;
    m_value = m_data[m_table_offset];
    return true;
  }
  return false;
}

bool ae_fade_table::get_state()
{
  return m_table_index == m_trigger_index;
}

void ae_fade_table::render()
{
  m_value = m_data[m_table_offset + m_table_index];
  if(m_trigger)
  {
    m_table_index++;
    if(m_table_index == m_trigger_index)
    {
      m_trigger = false;
    }
  }
}

void ae_fade_table::stop()
{
  m_table_index = 0;
  m_event = FadeEvent::None;
}

ae_fader::ae_fader()
{
}

void ae_fader::init(float *_fade_table)
{
  m_data[0] = &m_zero;
  m_data[1] = _fade_table;
  m_data[2] = &m_one;
  m_index = m_current = 2;
}

void ae_fader::pick(const uint32_t _index)
{
  m_index = _index;
  m_current = 1;
}

float ae_fader::get_value()
{
  return *m_data[m_current];
}

void ae_fader::stop()
{
  m_current = m_index;
}
