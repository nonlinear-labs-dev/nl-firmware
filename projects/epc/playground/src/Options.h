#pragma once

#include "playground.h"

class Options
{
 public:
  static bool s_acceptanceTests;

  Options();
  Options(int &argc, char **&argv);

  [[nodiscard]] const Glib::ustring &getPresetManagerPath() const;
  [[nodiscard]] const Glib::ustring &getSettingsFile() const;
  [[nodiscard]] const Glib::ustring &getBBBB() const;
  [[nodiscard]] const Glib::ustring &getLayoutFolder() const;
  [[nodiscard]] const Glib::ustring &getAudioEngineHost() const;
  [[nodiscard]] const Glib::ustring &getMidiBridge() const;
  [[nodiscard]] bool sendBBBBTurnaroundTimestamps() const;

  void setPresetManagerPath(const std::string& path);

 private:
  void setDefaults();
  void setDefaultsForTests();

  bool setPMPathName(const Glib::ustring &optionName, const Glib::ustring &path, bool hasValue);
  bool setLayoutFolder(const Glib::ustring &optionName, const Glib::ustring &path, bool hasValue);
  bool makePresetManagerDirectory(const Glib::RefPtr<Gio::File> &file);

  Glib::ustring m_pmPath;
  Glib::ustring m_settingsFile;
  Glib::ustring m_layoutFolder;
  Glib::ustring m_bbbb = "localhost";
  Glib::ustring m_audioEngineHost = "localhost";
  Glib::ustring m_midiBridge = "localhost";
  bool m_sendBBBTurnaroundTimes = false;
};
