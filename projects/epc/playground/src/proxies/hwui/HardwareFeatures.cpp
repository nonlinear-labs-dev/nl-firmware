#include "HardwareFeatures.h"
#include <tools/StringTools.h>
#include <fstream>

template <typename tEnum, typename tValue>
void matchAndSetEnum(std::vector<std::pair<tValue, tEnum>> stringEnumMapping, tEnum& e, tValue line)
{
  for(const auto& v : stringEnumMapping)
  {
    if(StringTools::contains(line, v.first))
    {
      e = v.second;
      break;
    }
  }
}

HardwareFeatures::HardwareFeatures()
{
  std::ifstream cpuInfo("/proc/cpuinfo");
  for(std::string line; std::getline(cpuInfo, line);)
  {
    if(StringTools::contains(line, "model name"))
    {
        matchAndSetEnum({ { "i3-5010", EPC_MODEL::i3_5 },
                        { "i3-7100", EPC_MODEL::i3_7 },
                        { "i5-7260", EPC_MODEL::i5_7 },
                        { "i3-1011", EPC_MODEL::i3_10 },
                        { "i3-1115", EPC_MODEL::i3_11 } },
                      m_model, line);
      break;
    }
  }

  switch(m_model)
  {
    case EPC_MODEL::UNKNOWN:
    case EPC_MODEL::i3_5:
    case EPC_MODEL::i3_7:
    default:
      m_hasEPCWifi = false;
      break;
    case EPC_MODEL::i5_7:
    case EPC_MODEL::i3_10:
    case EPC_MODEL::i3_11:
      m_hasEPCWifi = true;
      break;
  }
}

[[maybe_unused]] bool HardwareFeatures::hasEPCWiFi() const
{
  return m_hasEPCWifi;
}

EPC_MODEL HardwareFeatures::getModel() const
{
  return m_model;
}
