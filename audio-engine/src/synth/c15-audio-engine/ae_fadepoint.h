# pragma once

/******************************************************************************/
/** @file       ae_fadepoint.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      fadepoint generator
    @todo
*******************************************************************************/

#include "c15_config.h"
#include <vector>

class ae_fadepoint
{
public:
    ae_fadepoint();
    void init(const float _samplerate);
    void enable();
    float get_value();
    bool get_state();
    void render();
private:
    std::vector<float> m_data;
    uint32_t m_table_index, m_flush_index;
    bool m_flush;
};
