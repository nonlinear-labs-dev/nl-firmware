#pragma once
#include <device-settings/midi/mappings/MappingSetting.h>
#include <nltools/Types.h>
#include "RibbonCCMapping.h"

class Enable14BitSupport;

template <int tPedal> class PedalCCMapping : public MappingSetting<PedalCC>
{
 public:
  constexpr static auto PedalID = tPedal;

  PedalCCMapping(UpdateDocumentContributor& s, Enable14BitSupport& enable14Bit)
      : MappingSetting<PedalCC>(s, enable14Bit, getDefault(tPedal))
  {
    static_assert(PedalID > 0 && PedalID <= 4, "PedalCCMapping takes pedal template argument of 1,2,3 or 4");
  }

 private:
  static inline constexpr PedalCC getDefault(int pedal)
  {
    switch(pedal)
    {
      case 1:
        return PedalCC::CC20;
      case 2:
        return PedalCC::CC21;
      case 3:
        return PedalCC::CC22;
      case 4:
        return PedalCC::CC23;
      default:
        break;
    }
    nltools_detailedAssertAlways(pedal > 0 && pedal <= 4,
                                 "invalid template argument tPedal, out of bounds, use 1 to 4");
    return PedalCC::CC01;
  }

 public:
  const std::vector<Glib::ustring>& enumToString() const override
  {
    static auto ret = StringTools::convert(getAllStrings<PedalCC>());
    return ret;
  }

  const std::vector<Glib::ustring>& enumToDisplayString() const override
  {
    static std::vector<Glib::ustring> retWithCCs = { "None",
                                                     "CC 01/33",
                                                     "CC 02/34",
                                                     "CC 03/35",
                                                     "CC 04/36",
                                                     "CC 05/37",
                                                     "CC 06/38",
                                                     "CC 07/39",
                                                     "CC 08/40",
                                                     "CC 09/41",
                                                     "CC 10/42",
                                                     "CC 11/43",
                                                     "CC 12/44",
                                                     "CC 13/45",
                                                     "CC 14/46",
                                                     "CC 15/47",
                                                     "CC 16/48",
                                                     "CC 17/49",
                                                     "CC 18/50",
                                                     "CC 19/51",
                                                     "CC 20/52",
                                                     "CC 21/53",
                                                     "CC 22/54",
                                                     "CC 23/55",
                                                     "CC 24/56",
                                                     "CC 25/57",
                                                     "CC 26/58",
                                                     "CC 27/59",
                                                     "CC 28/60",
                                                     "CC 29/61",
                                                     "CC 30/62",
                                                     "CC 31/63",
                                                     "CC 64 (sw.)",
                                                     "CC 65 (sw.)",
                                                     "CC 66 (sw.)",
                                                     "CC 67 (sw.)",
                                                     "CC 68 (sw.)",
                                                     "CC 69 (sw.)",
                                                     "CC 64 (cont.)",
                                                     "CC 65 (cont.)",
                                                     "CC 66 (cont.)",
                                                     "CC 67 (cont.)",
                                                     "CC 68 (cont.)",
                                                     "CC 69 (cont.)"
                                                    };

    static std::vector<Glib::ustring> retWithoutLSB = { "None",
                                                        "CC 01",
                                                        "CC 02",
                                                        "CC 03",
                                                        "CC 04",
                                                        "CC 05",
                                                        "CC 06",
                                                        "CC 07",
                                                        "CC 08",
                                                        "CC 09",
                                                        "CC 10",
                                                        "CC 11",
                                                        "CC 12",
                                                        "CC 13",
                                                        "CC 14",
                                                        "CC 15",
                                                        "CC 16",
                                                        "CC 17",
                                                        "CC 18",
                                                        "CC 19",
                                                        "CC 20",
                                                        "CC 21",
                                                        "CC 22",
                                                        "CC 23",
                                                        "CC 24",
                                                        "CC 25",
                                                        "CC 26",
                                                        "CC 27",
                                                        "CC 28",
                                                        "CC 29",
                                                        "CC 30",
                                                        "CC 31",
                                                        "CC 64 (sw.)",
                                                        "CC 65 (sw.)",
                                                        "CC 66 (sw.)",
                                                        "CC 67 (sw.)",
                                                        "CC 68 (sw.)",
                                                        "CC 69 (sw.)",
                                                        "CC 64 (cont.)",
                                                        "CC 65 (cont.)",
                                                        "CC 66 (cont.)",
                                                        "CC 67 (cont.)",
                                                        "CC 68 (cont.)",
                                                        "CC 69 (cont.)"
                                                      };
    if(is14BitSupportEnabled())
      return retWithCCs;
    else
      return retWithoutLSB;
  }
};