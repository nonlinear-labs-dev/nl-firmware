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

// COMB IMPROVEMENTS

#define POTENTIAL_IMPROVEMENT_COMB_REDUCE_VOICE_LOOP_1 __POTENTIAL_IMPROVEMENT_DISABLED__
#define POTENTIAL_IMPROVEMENT_COMB_REDUCE_VOICE_LOOP_2 __POTENTIAL_IMPROVEMENT_DISABLED__
#define POTENTIAL_IMPROVEMENT_COMB_REDUCE_VOICE_LOOP_3 __POTENTIAL_IMPROVEMENT_DISABLED__
// - three options, potentially reducing voice loops within the Comb Filter to zero

// DNC IMPROVEMENTS

#define POTENTIAL_IMPROVEMENT_DNC_CONST_OF_ZERO __POTENTIAL_IMPROVEMENT_DISABLED__
// - first option just for proof of concept
#define POTENTIAL_IMPROVEMENT_DNC_OMIT_POLYPHONIC __POTENTIAL_IMPROVEMENT_DISABLED__
#define POTENTIAL_IMPROVEMENT_DNC_OMIT_MONOPHONIC __POTENTIAL_IMPROVEMENT_DISABLED__
// - two further options potentially saving many additions

// PARALLEL DATA SPECIFIC IMPROVEMENTS

//#define POTENTIAL_IMPROVEMENT_PARALLEL_DATA_REDUCE_LOOPS __POTENTIAL_IMPROVEMENT_DISABLED__
// - getting rid of Loops that could be fully parallel (first step: std::abs())

// TODO: TIME HANDLING IMPROVEMENTS

// - macro control times operate completely event based, as does the whole modulation mechanism (so no improvements here)
//#define POTENTIAL_IMPROVEMENT_TIME_IN_ENVELOPES __POTENTIAL_IMPROVEMENT_DISABLED__
//#define POTENTIAL_IMPROVEMENT_TIME_IN_MONO_GLIDE __POTENTIAL_IMPROVEMENT_DISABLED__
