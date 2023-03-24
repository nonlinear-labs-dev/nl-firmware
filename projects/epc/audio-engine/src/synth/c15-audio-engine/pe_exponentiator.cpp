#include "pe_exponentiator.h"

/* proper init */
void exponentiator::init()
{
  /* helper variables for initialization */
  uint32_t i;                         // array index variable
  float exp;                          // exponent variable
  /* construct linear pitch table */  // note: table construction loop goes up to
                                      // range element (as tables are set to be
                                      // range+1 in size)
  for(i = 0; i <= dsp_expon_lin_pitch_range; i++)
  {
    /* determine exponent of equation */
    exp = (static_cast<float>(i) + s_linear_pitch_from - s_freqExponent_offset) * s_scaleFreqExponent;
    /* determine power (base ^ exponent) */
    m_linear_pitch_table[i] = pow(s_freqBase, exp);
  }
  /* construct oscillator pitch table */
  m_oscillator_pitch_table[0] = 0.f;  // set first table element to zero
  /* construct hyperfloor part of oscillator pitch table (second till 19th
   * element) */
  for(i = 1; i < 20; i++)
  {
    /* determine exponent of equation */
    exp = (hyperfloor(static_cast<float>(i) + s_oscillator_pitch_from) - s_freqExponent_offset) * s_scaleFreqExponent;
    /* determine power (base ^ exponent) */
    m_oscillator_pitch_table[i] = pow(s_freqBase, exp);
  }
  /* construct remaining (linear) part of oscillator pitch table */
  for(i = 20; i <= dsp_expon_osc_pitch_range; i++)
  {
    /* determine exponent of equation */
    exp = (static_cast<float>(i) + s_oscillator_pitch_from - s_freqExponent_offset) * s_scaleFreqExponent;
    /* determine power (base ^ exponent) */
    m_oscillator_pitch_table[i] = pow(s_freqBase, exp);
  }
  /* construct level table */
  m_level_table[0] = 0.f;  // set first table element to zero
  for(i = 1; i <= dsp_expon_level_range; i++)
  {
    /* determine exponent of equation */
    exp = (static_cast<float>(i) + s_level_from) * s_scaleGainExponent;
    /* determine power (base ^ exponent) */
    m_level_table[i] = pow(s_gainBase, exp);
  }
  /* construct time table */
  m_time_table[0] = 0.f;  // set first table element to zero
  for(i = 1; i <= dsp_expon_time_range; i++)
  {
    /* determine exponent of equation */
    exp = (static_cast<float>(i) + s_time_from) * s_scaleGainExponent;
    /* determine power (base ^ exponent) */
    m_time_table[i] = pow(s_gainBase, exp);
  }
  /* construct loopFactor table */
  m_loopFactor_table[0] = 0.0f;
  const float ln = std::log(1.6f);
  for(i = 1; i < 99; i++)
  {
    const float y = 4.0f * std::exp(0.1f * ln * ((float) i - 50.0f));
    m_loopFactor_table[i] = pow(s_loop_factor_base, 1.0f / y);
  }
  while(i < env_loop_factor_range)
  {
    const float y = (404.04f * (float) i) - 39899.96f;
    m_loopFactor_table[i] = pow(s_loop_factor_base, 1.0f / y);
    i++;
  }
  m_loopFactor_table[env_loop_factor_range] = 1.0f;
}

/* helper methods */
float exponentiator::hyperfloor(float _value)
{
  /* the hyperbolic floor function of oscillator pitches */
  return ((s_hyperfloor[0] * _value) / (s_hyperfloor[1] + _value));
}
