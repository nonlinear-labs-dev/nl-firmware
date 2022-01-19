#pragma once

#include <device-settings/BooleanSetting.h>

class FlacRecorderVirgin : public BooleanSetting
{
 public:
  FlacRecorderVirgin(Settings& s);

 private:
  sigc::connection m_connection;
};
