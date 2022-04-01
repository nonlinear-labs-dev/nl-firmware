#pragma once
#include <device-settings/midi/mappings/MappingSetting.h>
#include <nltools/Types.h>
#include <tools/StringTools.h>

class AftertouchCCMapping : public MappingSetting<AftertouchCC>
{
 public:
  AftertouchCCMapping(UpdateDocumentContributor& u, Enable14BitSupport& enable14Bit)
      : MappingSetting<AftertouchCC>(u, enable14Bit, AftertouchCC::ChannelPressure)
  {
  }

  const std::vector<Glib::ustring>& enumToString() const override
  {
    static auto ret = StringTools::convert(getAllStrings<AftertouchCC>());
    return ret;
  }

  const std::vector<Glib::ustring>& enumToDisplayString() const override
  {
    static std::vector<Glib::ustring> ret = { "None",
                                              "Channel Pressure",
                                              "CC 01 (LSB: CC 33)",
                                              "CC 02 (LSB: CC 34)",
                                              "CC 03 (LSB: CC 35)",
                                              "CC 04 (LSB: CC 36)",
                                              "CC 05 (LSB: CC 37)",
                                              "CC 06 (LSB: CC 38)",
                                              "CC 07 (LSB: CC 39)",
                                              "CC 08 (LSB: CC 40)",
                                              "CC 09 (LSB: CC 41)",
                                              "CC 10 (LSB: CC 42)",
                                              "CC 11 (LSB: CC 43)",
                                              "CC 12 (LSB: CC 44)",
                                              "CC 13 (LSB: CC 45)",
                                              "CC 14 (LSB: CC 46)",
                                              "CC 15 (LSB: CC 47)",
                                              "CC 16 (LSB: CC 48)",
                                              "CC 17 (LSB: CC 49)",
                                              "CC 18 (LSB: CC 50)",
                                              "CC 19 (LSB: CC 51)",
                                              "CC 20 (LSB: CC 52)",
                                              "CC 21 (LSB: CC 53)",
                                              "CC 22 (LSB: CC 54)",
                                              "CC 23 (LSB: CC 55)",
                                              "CC 24 (LSB: CC 56)",
                                              "CC 25 (LSB: CC 57)",
                                              "CC 26 (LSB: CC 58)",
                                              "CC 27 (LSB: CC 59)",
                                              "CC 28 (LSB: CC 60)",
                                              "CC 29 (LSB: CC 61)",
                                              "CC 30 (LSB: CC 62)",
                                              "CC 31 (LSB: CC 63)",
                                              "Pitchbend up",
                                              "Pitchbend down" };

    static std::vector<Glib::ustring> retWithoutLSB = { "None",          "Channel Pressure",
                                                        "CC 01",         "CC 02",
                                                        "CC 03",         "CC 04",
                                                        "CC 05",         "CC 06",
                                                        "CC 07",         "CC 08",
                                                        "CC 09",         "CC 10",
                                                        "CC 11",         "CC 12",
                                                        "CC 13",         "CC 14",
                                                        "CC 15",         "CC 16",
                                                        "CC 17",         "CC 18",
                                                        "CC 19",         "CC 20",
                                                        "CC 21",         "CC 22",
                                                        "CC 23",         "CC 24",
                                                        "CC 25",         "CC 26",
                                                        "CC 27",         "CC 28",
                                                        "CC 29",         "CC 30",
                                                        "CC 31",         "Pitchbend up",
                                                        "Pitchbend down" };
    if(is14BitSupportEnabled())
      return ret;
    else
      return retWithoutLSB;
  }
};
