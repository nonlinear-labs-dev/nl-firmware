#include <glibmm/ustring.h>
#include "HardwareFeatures.h"
#include "nltools/system/SpawnCommandLine.h"
#include <tools/StringTools.h>

HardwareFeatures::HardwareFeatures()
{
  SpawnCommandLine cmd("cat /proc/cpuinfo");
  Glib::ustring cpuInfo = cmd.getStdOutput();
  auto modelName = StringTools::grepFirstLineWithOccurrence(cpuInfo, "model name");
  if(StringTools::contains(modelName, "i3-5010"))
  {
    m_model = EPC_MODEL::i3_5;
  }
  else if(StringTools::contains(modelName, "i3-7100"))
  {
    m_model = EPC_MODEL::i3_7;
  }
  else if(StringTools::contains(modelName, "i3-1011"))
  {
    m_model = EPC_MODEL::i3_10;
  }
  else if(StringTools::contains(modelName, "i3-1115"))
  {
    m_model = EPC_MODEL::i3_11;
  }

  switch(m_model)
  {
    case EPC_MODEL::UNKNOWN:
    case EPC_MODEL::i3_5:
    case EPC_MODEL::i3_7:
      m_hasEPCWifi = false;
      break;
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
