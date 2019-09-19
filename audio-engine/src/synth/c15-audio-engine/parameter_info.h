#pragma once

/******************************************************************************/
/** @file       parameter_info.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      describing all necessary parameter-related details
    @todo
*******************************************************************************/

namespace C15
{

// naming every parameter (according to its type)
namespace Parameters
{

// all global Hardware Sources
enum class Hardware_Sources
{
    Pedal_1, Pedal_2, Pedal_3, Pedal_4, Bender, Aftertouch, Ribbon_1, Ribbon_2,
    _LENGTH_
};

// all other global Parameters
enum class Global_Parameters
{
    _LENGTH_
};

// all local Hardware Amounts
enum class Hardware_Amounts
{
    Pedal_1_to_MC_A, Pedal_2_to_MC_A, Pedal_3_to_MC_A, Pedal_4_to_MC_A, Bender_to_MC_A, Aftertouch_to_MC_A, Ribbon_1_to_MC_A, Ribbon_2_to_MC_A,
    Pedal_1_to_MC_B, Pedal_2_to_MC_B, Pedal_3_to_MC_B, Pedal_4_to_MC_B, Bender_to_MC_B, Aftertouch_to_MC_B, Ribbon_1_to_MC_B, Ribbon_2_to_MC_B,
    Pedal_1_to_MC_C, Pedal_2_to_MC_C, Pedal_3_to_MC_C, Pedal_4_to_MC_C, Bender_to_MC_C, Aftertouch_to_MC_C, Ribbon_1_to_MC_C, Ribbon_2_to_MC_C,
    Pedal_1_to_MC_D, Pedal_2_to_MC_D, Pedal_3_to_MC_D, Pedal_4_to_MC_D, Bender_to_MC_D, Aftertouch_to_MC_D, Ribbon_1_to_MC_D, Ribbon_2_to_MC_D,
    _LENGTH_
};

// all local Macro Controls (including None)
enum class Macro_Controls
{
    None, MC_A, MC_B, MC_C, MC_D,
    _LENGTH_
};

// all local Macro Control Times (also including None - maintaining structure)
enum class Macro_Times
{
    None, MC_A, MC_B, MC_C, MC_D,
    _LENGTH_
};

// all local "Target" Parameters (modulateable)
enum class Modulateable_Parameters
{
    _LENGTH_
};

// all local "Direct" Parameters (unmodulateable)
enum class Unmodulateable_Parameters
{
    _LENGTH_
};

} // namespace C15::Parameters

// naming every smoother (according to its section and clock affiliation)
namespace Smoothers
{

enum class Global_Sync
{
    _LENGTH_
};

enum class Global_Audio
{
    _LENGTH_
};

enum class Global_Fast
{
    _LENGTH_
};

enum class Global_Slow
{
    _LENGTH_
};

enum class Poly_Sync
{
    _LENGTH_
};

enum class Poly_Audio
{
    _LENGTH_
};

enum class Poly_Fast
{
    _LENGTH_
};

enum class Poly_Slow
{
    _LENGTH_
};

enum class Mono_Sync
{
    _LENGTH_
};

enum class Mono_Audio
{
    _LENGTH_
};

enum class Mono_Fast
{
    _LENGTH_
};

enum class Mono_Slow
{
    _LENGTH_
};

} // namespace C15::Smoothers

// naming every signal (according to its section affiliation)
namespace Signals
{

// all (monophonic) global-section signals
enum class Global_Signals
{
    _LENGTH_
};

// all (monophonic) poly-section signals
enum class Quasipoly_Signals
{
    _LENGTH_
};

// all (polyphonic) poly-section signals
enum class Truepoly_Signals
{
    _LENGTH_
};

// all (monophonic) mono-section signals
enum class Mono_Signals
{
    _LENGTH_
};

} // namespace C15::Signals

// naming every scale method
enum class Scale
{
    None
};

} // namespace C15
