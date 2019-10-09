#pragma once

/******************************************************************************/
/** @file       c15_config.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      crucial information about the audio engine is now kept here
    @todo
*******************************************************************************/

#include <stdint.h>

namespace C15
{

namespace Config
{

const char milestone[8] = "1.7B";
const uint32_t total_polyphony = 24;
const uint32_t local_polyphony = total_polyphony >> 1;
const uint32_t tcd_elements = 1; // 328 (since 1.57)
const uint32_t clock_rates[4] = {0, 48000, 9600, 400};
const float fade_time_ms = 3.0f;

const uint32_t key_count = 61;
const uint32_t key_center = 36;

} // namespace C15::Config

} // namespace C15

