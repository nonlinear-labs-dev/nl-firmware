#pragma once

/******************************************************************************/
/**	@file       placeholder.h
    @date       2022-06-07, 17:24
    @version    1.7B-9
    @author     M. Seeber
    @brief      placeholders for parameter label/info string replacements
    @todo       generate from definition (current version is hand-coded)
*******************************************************************************/

#include "../src/placeholder.h"

namespace C15
{

  namespace Placeholder
  {

    // common
    static constexpr Text VgI = u8"\ue071";
    static constexpr Text VgII = u8"\ue072";

    static constexpr GlobalReplacer GlobalPlaceholders[]
        = { { "@MC:A", u8"\ue000" }, { "@MC:B", u8"\ue001" }, { "@MC:C", u8"\ue002" },    { "@MC:D", u8"\ue003" },
            { "@MC:E", u8"\ue200" }, { "@MC:F", u8"\ue201" }, { "@ST:Pan", "FX", "Part" } };

    // common local
    static constexpr auto VgSelf = Replacer::createVoiceGroupReplacer({ VgI, VgII });
    static constexpr auto VgOther = Replacer::createVoiceGroupReplacer({ VgII, VgI });
    static constexpr auto VgBetween = Replacer::createVoiceGroupReplacer({ u8"\ue071-\ue072" });

    static constexpr LocalReplacer LocalPlaceholders[] = {
      { "@VG:Self", VgSelf },
      { "@VG:Other", VgOther },
      { "@ST:Between", VgBetween, VgOther }
      /* parameters to come */
      //    { "@ST:FX", { u8"\ue072 to \ue071", u8"\ue071 to \ue072" } }
    };

  }  // namespace C15::Placeholder

}  // namespace C15
