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

// actual parameter TCD-order list definition: { Type, ScaleId, ScaleArg, Initial, Polarity, Smoother, Signal }
// besides type, all descriptors only refer to modulateable and unmodulateable parameters (so, "mod" params only need type description)
// ignored parameters can be described via "None" (unused/ignored TCD ids)

const ParameterDescriptor ParameterList[Config::tcd_elements] = {
    None
};

} // namespace C15
