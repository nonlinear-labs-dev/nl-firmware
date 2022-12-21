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
class HardwareFeatures;
class EditBuffer;

class Settings : public UpdateDocumentContributor
{
 public:
  typedef std::unique_ptr<Setting> tSettingPtr;
  typedef std::map<Glib::ustring, tSettingPtr> tMap;

  explicit Settings(const Glib::ustring &file, UpdateDocumentMaster *master, HardwareFeatures& hwFeatures);
  ~Settings() override;

  void init(EditBuffer* eb);
  void reload();

  Setting *getSetting(const Glib::ustring &key);
  void addSetting(const Glib::ustring &key, Setting *s);

  template <typename T> T *getSetting()
  {
    for(auto &s : m_settings)
      if(auto r = dynamic_cast<T *>(s.second.get()))
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
