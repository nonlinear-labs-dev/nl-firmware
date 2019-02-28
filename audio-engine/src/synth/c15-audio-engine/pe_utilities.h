/******************************************************************************/
/** @file		pe_utilities.h
    @date       2018-03-13
    @version    1.0
    @author     Matthias Seeber
    @brief		quasi-dynamic list objects necessary for ID handling
                (tcd_decoder, param_engine)
                (currently, no debugging capabilities/functions present)
    @todo
*******************************************************************************/

#pragma once

#include <stdint.h>
#include "pe_defines_config.h"

struct id_list
{
    /* local variables */
    uint32_t m_data[sig_number_of_params];
    uint32_t m_length = 0;
    /* list operations */
    void reset();
    void add(const uint32_t _id);
};

struct dual_id_list
{
    /* local variables */
    id_list m_data[2];
    /* list operations */
    void reset();
    void add(const uint32_t _listId, const uint32_t _id);
};

struct polyDual_id_list
{
    /* local variables */
    dual_id_list m_data[dsp_poly_types];
    /* list operations */
    void reset();
    void add(const uint32_t _polyId, const uint32_t _listId, const uint32_t _id);
};

struct poly_id_list
{
    /* local variables */
    id_list m_data[dsp_poly_types];
    /* list operations */
    void reset();
    void add(const uint32_t _polyId, const uint32_t _id);
};

struct clock_id_list
{
    /* local variables */
    poly_id_list m_data[dsp_clock_types];
    /* list operations */
    void reset();
    void add(const uint32_t _clockId, const uint32_t _polyId, const uint32_t _id);
};

struct dual_clock_id_list
{
    /* local variables */
    clock_id_list m_data[2];
    /* list operations */
    void reset();
    void add(const uint32_t _listId, const uint32_t _clockId, const uint32_t _polyId, const uint32_t _id);
};

struct env_id_list
{
    /* local variables */
    uint32_t m_data[sig_number_of_envelopes];
    uint32_t m_length = 0;
    /* list operations */
    void reset();
    void add(const uint32_t _id);
};

struct dual_env_id_list
{
    /* local variables */
    env_id_list m_data[2];
    /* list operations */
    void reset();
    void add(const uint32_t _listId, const uint32_t _id);
};
