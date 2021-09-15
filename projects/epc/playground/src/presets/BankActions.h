#pragma once

#include <http/RPCActionManager.h>
#include <libundo/undo/Scope.h>
#include <xml/FileInStream.h>
#include <http/SectionAndActionManager.h>

class PresetManager;
class Preset;
class Bank;
class Settings;

class BankActions : public SectionAndActionManager
{
 public:
  explicit BankActions(UpdateDocumentContributor* parent, PresetManager &presetManager, Settings &settings);
  ~BankActions() override;

  bool handleRequest(const Glib::ustring &path, std::shared_ptr<NetworkRequest> request) override;
 private:
  typedef Preset *tPresetPtr;

  void insertBank(const std::shared_ptr<NetworkRequest>& request, size_t offset);
  PresetManager &m_presetManager;
  Settings& m_settings;
};
