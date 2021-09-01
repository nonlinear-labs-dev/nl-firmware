#pragma once

#include <http/RPCActionManager.h>
#include <libundo/undo/Scope.h>
#include <xml/FileInStream.h>

class PresetManager;
class Preset;
class Bank;

class BankActions : public RPCActionManager
{
  typedef RPCActionManager super;

 public:
  explicit BankActions(PresetManager &presetManager);
  ~BankActions() override;

  bool handleRequest(const Glib::ustring &path, std::shared_ptr<NetworkRequest> request) override;

 private:
  typedef Preset *tPresetPtr;

  void insertBank(Bank *bank, Bank *targetBank, size_t insertPos);
  void insertBank(const std::shared_ptr<NetworkRequest>& request, size_t offset);

  PresetManager &m_presetManager;
};
