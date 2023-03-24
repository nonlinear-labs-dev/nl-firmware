#pragma once
#include <sigc++/sigc++.h>

class HWUI;
class ScreenSaverTimeoutSetting;
class EditBuffer;
class PlaycontrollerProxy;
class Settings;

class ScreenSaverConnections : public sigc::trackable
{
 public:
  explicit ScreenSaverConnections(ScreenSaverTimeoutSetting& setting);
  void connectRescheduleToSources(HWUI& hwui, EditBuffer& eb, PlaycontrollerProxy& pcp, Settings& settings);

 private:
  ScreenSaverTimeoutSetting& m_setting;
};
