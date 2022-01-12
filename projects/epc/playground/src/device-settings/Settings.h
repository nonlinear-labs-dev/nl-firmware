#pragma once

#include "playground.h"
#include "http/ContentSection.h"
#include <tools/DelayedJob.h>
#include <tools/ScopedGuard.h>
#include <utility>
#include <map>
#include "SendReason.h"
#include <nltools/Types.h>

class Application;
class Setting;
class SettingsActions;

class Settings : public UpdateDocumentContributor
{
 public:
  typedef std::shared_ptr<Setting> tSettingPtr;
  typedef std::map<Glib::ustring, tSettingPtr> tMap;

  explicit Settings(const Glib::ustring &file, UpdateDocumentMaster *master);
  ~Settings() override;

  void init();
  void reload();

  tSettingPtr getSetting(const Glib::ustring &key);
  void addSetting(const Glib::ustring &key, Setting *s);

  template <typename T> std::shared_ptr<T> getSetting()
  {
    for(auto &s : m_settings)
      if(std::shared_ptr<T> r = std::dynamic_pointer_cast<T>(s.second))
        return r;

    return nullptr;
  }

  const tMap &getSettings() const;

  void sendSettingsToPlaycontroller(SendReason reason);
  void sendGlobalPlaycontrollerInitSettings();
  void sendPresetSettingsToPlaycontroller();

  tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;
  sigc::connection onSettingsChanged(sigc::slot<void(void)> s);

  bool isLoading() const;
  void writeDocument(Writer &writer, tUpdateID knownRevision) const;

 private:
  void save();
  void load();
  void sanitize();

  Glib::ustring m_file;
  tMap m_settings;
  DelayedJob m_saveJob;
  ScopedGuard m_isLoading;
  void sendGlobalAESettings();
  sigc::connection m_aeSettingsConnection;
  sigc::signal<void(void)> m_sigChanged;
};
