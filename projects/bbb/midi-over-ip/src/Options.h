#pragma once

#include <glibmm/ustring.h>

class Options
{
 public:
  Options(int &argc, char **&argv);

  Glib::ustring getAudioEngineHost() const;
  Glib::ustring getMidiDevice() const;

 private:
  Glib::ustring m_audioengineHost = "192.168.10.10";
  Glib::ustring m_midiDevice = "hw:0,0";
};
