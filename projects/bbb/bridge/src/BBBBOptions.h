#pragma once

#include "bbbb.h"
#include <glibmm/ustring.h>

class BBBBOptions
{
 public:
  BBBBOptions(int &argc, char **&argv);

  Glib::ustring getPlaygroundHost() const;
  Glib::ustring getAudioEngineHost() const;

  bool doTimeStamps() const;
  bool logPlaycontrollerRaw() const;
  bool logHeartBeat() const;

  Glib::ustring getFromPlaycontrollerPath() const;
  Glib::ustring getToPlaycontrollerPath() const;

 private:
  bool m_doTimeStamps = false;
  bool m_logPlaycontrollerRaw = false;
  bool m_logHeartBeat = false;

  Glib::ustring m_toPlaycontrollerDriverPath = "/dev/lpc_bb_driver";
  Glib::ustring m_fromPlaycontrollerDriverPath = "/dev/lpc_bb_driver";
  Glib::ustring m_playgroundHost = "localhost";
  Glib::ustring m_audioengineHost = "localhost";
};
