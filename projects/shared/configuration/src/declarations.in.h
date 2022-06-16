#pragma once

/******************************************************************************/
/** @file       declarations.h
    @date       ${timestamp}
    @version    ${config.milestone}-${config.version}
    @author     M. Seeber
    @brief      descriptors for all parameter-related details
    @todo       
*******************************************************************************/

namespace C15
{
    namespace Descriptors {
        enum class ParameterType {
            ${enums.parameter_type}
        };
    } // namespace C15::Descriptors
    namespace Properties {} // namespace C15::Properties
    namespace Parameters {} // namespace C15::Parameters
    namespace Smoothers {} // namespace C15::Smoothers
    namespace Signals {} // namespace C15::Signals
} // namespace C15
