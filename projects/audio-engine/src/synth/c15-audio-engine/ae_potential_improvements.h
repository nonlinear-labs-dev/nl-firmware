#pragma once

/******************************************************************************/
/** @file       ae_potential_improvements.h
    @date       2020-04-22
    @version    1.7B-4
    @author     M. Seeber

    @brief      contains define flags enabling potential dsp improvements

                - primarily attempting to lower permanent cpu usage
                - if possible, maybe reduce some event-related cpu usage as well

                (this file is intended to only be temporary and should be deleted after everything is implemented, tested, decided and cleaned up)

    @todo
*******************************************************************************/

// two basic states (disabled, enabled) are provided (disabled refers to the current implementation)

#define __POTENTIAL_IMPROVEMENT_DISABLED__ 0
#define __POTENTIAL_IMPROVEMENT_ENABLED__ 1

// each potential improvent can now be disabled/enabled individually

#define POTENTIAL_IMPROVEMENT_COMB_REDUCE_VOICE_LOOP_1 __POTENTIAL_IMPROVEMENT_DISABLED__
#define POTENTIAL_IMPROVEMENT_COMB_REDUCE_VOICE_LOOP_2 __POTENTIAL_IMPROVEMENT_DISABLED__
