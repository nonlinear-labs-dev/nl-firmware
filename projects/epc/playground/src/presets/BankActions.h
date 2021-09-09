#pragma once

#include <http/RPCActionManager.h>
#include <libundo/undo/Scope.h>
#include <xml/FileInStream.h>

class PresetManager;
class Preset;
class Bank;

class BankActions : public RPCActionManager
{
 public:
  explicit BankActions(PresetManager &presetManager);
  ~BankActions() override;

  bool handleRequest(const Glib::ustring &path, std::shared_ptr<NetworkRequest> request) override;
 private:
  typedef Preset *tPresetPtr;

  void insertBank(const std::shared_ptr<NetworkRequest>& request, size_t offset);
  PresetManager &m_presetManager;
};
