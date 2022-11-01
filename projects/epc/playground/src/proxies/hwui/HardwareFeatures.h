#pragma once
#include "nltools/Types.h"

class HardwareFeatures
{
 public:
  HardwareFeatures();
  [[nodiscard]] bool hasEPCWiFi() const;
  [[nodiscard]] EPC_MODEL getModel() const;

 private:
  EPC_MODEL m_model = EPC_MODEL::UNKNOWN;
  bool m_hasEPCWifi = false;
};
