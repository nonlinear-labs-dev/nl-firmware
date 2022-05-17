#pragma once

/******************************************************************************/
/**	@file       placeholder.h
    @date       2022-05-17, 14:48
    @version    1.7B-9
    @author     M. Seeber
    @brief      placeholders for parameter label/info string replacements
    @todo       generate from definition (current version is hand-coded)
*******************************************************************************/

#include "../src/placeholder.h"

namespace C15
{

namespace Placeholder {

static constexpr GlobalReplacer GlobalPlaceholders[] = {
    { "@MC:A", u8"\ue000" },
    { "@MC:B", u8"\ue001" },
    { "@MC:C", u8"\ue002" },
    { "@MC:D", u8"\ue003" },
    { "@MC:E", u8"\ue200" },
    { "@MC:F", u8"\ue201" }
};

// common local
static constexpr Text VgI = u8"\ue081";
static constexpr Text VgII = u8"\ue082";
static constexpr auto VgSelf = Replacer::createVoiceGroupReplacer( { VgI, VgII } );
static constexpr auto VgOther = Replacer::createVoiceGroupReplacer( { VgII, VgI } );
static constexpr auto VgBetween = Replacer::createVoiceGroupReplacer( { u8"\ue081-\ue082" } );

static constexpr LocalReplacer LocalPlaceholders[] = {
    { "@VG:Self", VgSelf },
    { "@VG:Other", VgOther },
    { "@ST:From", VgBetween, VgOther },
    { "@ST:To", VgBetween, VgOther }
/* parameters to come */
//    { "@ST:Pan", { "FX" }, { "Part" } },
//    { "@ST:FX", { u8"\ue082 to \ue081", u8"\ue081 to \ue082" } }
};

} // namespace C15::Placeholder

} // namespace C15
