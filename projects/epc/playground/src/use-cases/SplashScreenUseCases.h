#pragma once
#include <glibmm/ustring.h>

class HWUI;
class Settings;
class SplashLayout;

class SplashScreenUseCases
{
 public:
  SplashScreenUseCases(HWUI& hwui, Settings& settings);
  void startSplashScreen();
  void addSplashScreenMessage(const Glib::ustring& string);
  void setSplashScreenMessage(const Glib::ustring& string);
  void finishSplashScreen();

 private:
  SplashLayout* getSplashScreenOverlay();
  HWUI& m_hwui;
  Settings& m_settings;
};
