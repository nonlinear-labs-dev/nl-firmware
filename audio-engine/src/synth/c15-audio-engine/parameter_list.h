#pragma once

/******************************************************************************/
/** @file       parameter_list.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      a more flexible parameter list (still TCD order)
    @todo
*******************************************************************************/

#include "c15_config.h"
#include "parameter_descriptor.h"

namespace C15
{

// actual parameter list definition
const ParameterDescriptor ParameterList[Config::tcd_elements] = {
    None
};

} // namespace C15
