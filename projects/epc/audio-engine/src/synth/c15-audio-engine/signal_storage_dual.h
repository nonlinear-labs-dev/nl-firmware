#pragma once

/******************************************************************************/
/** @file       signal_storage_dual.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new handles for audio engine signals
                (usable with numeric and enum class indices)
    @todo
*******************************************************************************/

#include <c15_config.h>
#include "ParallelData.h"

using PolyValue = ParallelData<float, C15::Config::local_polyphony>;
using PolyInt = ParallelData<int32_t, C15::Config::local_polyphony>;

struct LayerSignalCollection
{
  PolyValue m_osc_a = {}, m_osc_b = {}, m_comb = {}, m_svf = {};
  float m_fx_dry = 0.0f, m_fx_wet = 0.0f, m_fx_l = 0.0f, m_fx_r = 0.0f;
  inline void reset()
  {
    m_osc_a = m_osc_b = m_comb = m_svf = 0.0f;
    m_fx_dry = m_fx_wet = 0.0f, m_fx_l = 0.0f, m_fx_r = 0.0f;
  }
};

// global and mono sections require mono signals
template <class M> class MonoSignalStorage
{
 public:
  // numeric and enum-class-based getters
  inline const float& get(const uint32_t _id)
  {
    return m_data[_id];
  }
  inline const float& get(const M _id)
  {
    return m_data[static_cast<uint32_t>(_id)];
  }
  // numeric and enum-class-based setters
  inline void set(const uint32_t _id, const float _value)
  {
    m_data[_id] = _value;
  }
  inline void set(const M _id, const float _value)
  {
    m_data[static_cast<uint32_t>(_id)] = _value;
  }
  // reset (if needed)
  inline void reset()
  {
    for(uint32_t i = 0; i < static_cast<uint32_t>(M::_LENGTH_); i++)
    {
      m_data[i] = 0.0f;
    }
  }

 private:
  float m_data[static_cast<uint32_t>(M::_LENGTH_)] = {};
};

// poly sections require poly and mono signals
template <class P, class M> class PolySignalStorage
{
 public:
  // numeric and enum-class-based getters (mono, poly)
  inline const float& get_mono(const uint32_t _id)
  {
    return m_mono[_id];
  }
  inline const float& get(const M _id)
  {
    return m_mono[static_cast<uint32_t>(_id)];
  }
  inline const float& get_poly(const uint32_t _id, const uint32_t _voiceId)
  {
    return m_poly[_id][_voiceId];
  }
  inline const float& get(const P _id, const uint32_t _voiceId)
  {
    return m_poly[static_cast<uint32_t>(_id)][_voiceId];
  }
  inline const PolyValue& get_poly(const uint32_t _id)
  {
    return m_poly[_id];
  }
  inline const PolyValue& get(const P _id)
  {
    return m_poly[static_cast<uint32_t>(_id)];
  }
  // numeric and enum-class-based setters (mono, poly)
  inline void set_mono(const uint32_t _id, const float _value)
  {
    m_mono[_id] = _value;
  }
  inline void set(const M _id, const float _value)
  {
    m_mono[static_cast<uint32_t>(_id)] = _value;
  }
  inline void set_poly(const uint32_t _id, const uint32_t _voiceId, const float _value)
  {
    m_poly[_id][_voiceId] = _value;
  }
  inline void set_poly(const P _id, const uint32_t _voiceId, const float _value)
  {
    m_poly[static_cast<uint32_t>(_id)][_voiceId] = _value;
  }
  inline void set(const P _id, const uint32_t _voiceId, const float _value)
  {
    m_poly[static_cast<uint32_t>(_id)][_voiceId] = _value;
  }
  inline void set_poly(const uint32_t _id, const PolyValue& _value)
  {
    m_poly[_id] = _value;
  }
  inline void set(const P _id, const PolyValue& _value)
  {
    m_poly[static_cast<uint32_t>(_id)] = _value;
  }
  // reset (if needed)
  inline void reset()
  {
    for(uint32_t i = 0; i < static_cast<uint32_t>(M::_LENGTH_); i++)
    {
      m_mono[i] = 0.0f;
    }
    for(uint32_t i = 0; i < static_cast<uint32_t>(P::_LENGTH_); i++)
    {
      m_poly[i] = 0.0f;
    }
  }

 private:
  PolyValue m_poly[static_cast<uint32_t>(P::_LENGTH_)] = {};
  float m_mono[static_cast<uint32_t>(M::_LENGTH_)] = {};
};
