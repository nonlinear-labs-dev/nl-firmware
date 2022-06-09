#pragma once

/******************************************************************************/
/** @file       config.h
    @date       ${timestamp}
    @version    1.7B-9
    @author     M. Seeber
    @brief      crucial information about the audio engine
    @todo       
*******************************************************************************/

#include <stdint.h>

namespace C15
{

    namespace Config
    {
        constexpr char const * const milestone = "${config.milestone}";
        constexpr uint32_t total_polyphony = ${config.max_voices};
        constexpr uint32_t local_polyphoy = total_polyphony >> 1;
    }

}