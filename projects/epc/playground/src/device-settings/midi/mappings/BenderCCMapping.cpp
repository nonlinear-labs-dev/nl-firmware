#include "BenderCCMapping.h"
#include <tools/StringTools.h>

BenderCCMapping::BenderCCMapping(UpdateDocumentContributor& u, Enable14BitSupport& enable14Bit)
    : MappingSetting<BenderCC>(u, enable14Bit, BenderCC::Pitchbend)
{
}

const std::vector<Glib::ustring>& BenderCCMapping::enumToString() const
{
  static auto ret = StringTools::convert(getAllStrings<BenderCC>());
  return ret;
}

const std::vector<Glib::ustring>& BenderCCMapping::enumToDisplayString() const
{
  static std::vector<Glib::ustring> ret = { "None",
                                            "Pitchbend",
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
                                            "CC 31/63" };

  static std::vector<Glib::ustring> retWithoutLSB
      = { "None",  "Pitchbend", "CC 01", "CC 02", "CC 03", "CC 04", "CC 05", "CC 06", "CC 07", "CC 08", "CC 09",
          "CC 10", "CC 11",     "CC 12", "CC 13", "CC 14", "CC 15", "CC 16", "CC 17", "CC 18", "CC 19", "CC 20",
          "CC 21", "CC 22",     "CC 23", "CC 24", "CC 25", "CC 26", "CC 27", "CC 28", "CC 29", "CC 30", "CC 31" };

  if(is14BitSupportEnabled())
    return ret;
  else
    return retWithoutLSB;
}
