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

ae_fadepoint::ae_fadepoint()
{}

void ae_fadepoint::init(const float _samplerate)
{
    float
        sample_interval = 1.0f / _samplerate,
        fade_time = 1e-3f * C15::Config::fade_time_ms * _samplerate;

    m_table_index = 0;
    m_flush = false;
    m_data.resize((2 * static_cast<uint32_t>(fade_time)) + 2);
    m_flush_index = static_cast<uint32_t>((m_data.size() - 1) / 2);

    for(uint32_t i = 0; i < m_data.size(); i++)
    {
        if(i == m_flush_index + 1)
        {
            m_data[i] = m_data[i - 1];
        }
        else if(i < m_flush_index)
        {
            float x = std::cos(1.5708f * sample_interval * static_cast<float>(i) / fade_time);
            m_data[i] = x * x;
        }
        else if(i > m_flush_index)
        {
            float x = std::cos(1.5708f * sample_interval * static_cast<float>(i - 1) / fade_time);
            m_data[i] = x * x;
        }
    }
}

void ae_fadepoint::enable()
{
    m_flush = true;
}

float ae_fadepoint::get_value()
{
    return m_data[m_table_index];
}

bool ae_fadepoint::get_state()
{
    return m_table_index == m_flush_index;
}

void ae_fadepoint::render()
{
    if(m_flush)
    {
        m_table_index++;
        if(m_table_index == m_data.size())
        {
            m_table_index = 0;
            m_flush = false;
        }
    }
}
