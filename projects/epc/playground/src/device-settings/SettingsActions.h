#pragma once

#include <presets/PresetManager.h>
#include <http/SectionAndActionManager.h>
#include "http/RPCActionManager.h"

class Settings;

class SettingsActions : public SectionAndActionManager
{
 public:
  explicit SettingsActions(UpdateDocumentContributor* parent, Settings& settings, PresetManager& pm);
  ~SettingsActions() override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;
  SettingsActions(const SettingsActions& other) = delete;
  SettingsActions& operator=(const SettingsActions&) = delete;
 private:
  Settings& m_settings;
};
