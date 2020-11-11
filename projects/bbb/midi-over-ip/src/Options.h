#pragma once

#include <glibmm/ustring.h>

class Options
{
 public:
  Options(int &argc, char **&argv);

  Glib::ustring getAudioEngineHost() const;

  Glib::ustring getMidiInDevice() const;
  Glib::ustring getMidiOutDevice() const;

 private:
  Glib::ustring m_audioengineHost = "192.168.10.10";
  Glib::ustring m_iDevice;
  Glib::ustring m_oDevice;
};
