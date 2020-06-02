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
  bool logLPCRaw() const;
  bool logHeartBeat() const;

  Glib::ustring getFromLPCPath() const;
  Glib::ustring getToLPCPath() const;

 private:
  bool m_doTimeStamps = false;
  bool m_logLpcRaw = false;
  bool m_logHeartBeat = false;

  Glib::ustring m_toLpcDriverPath = "/dev/lpc_bb_driver";
  Glib::ustring m_fromLpcDriverPath = "/dev/lpc_bb_driver";
  Glib::ustring m_playgroundHost = "localhost";
  Glib::ustring m_audioengineHost = "localhost";
};
