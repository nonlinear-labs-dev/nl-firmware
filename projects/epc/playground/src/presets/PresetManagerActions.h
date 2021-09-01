#pragma once

#include "http/RPCActionManager.h"
#include "use-cases/PresetManagerUseCases.h"
#include <libsoup/soup-message-body.h>
#include <use-cases/SoundUseCases.h>

namespace UNDO
{
  class Transaction;
}

class PresetManager;
class Preset;
class HTTPRequest;
class AudioEngineProxy;

class PresetManagerActions : public RPCActionManager
{
 private:
  typedef RPCActionManager super;
  PresetManager& m_presetManager;
  PresetManagerUseCases pmUseCases;
  SoundUseCases soundUseCases;
  AudioEngineProxy& m_aeProxy;

 public:
  explicit PresetManagerActions(PresetManager& presetManager, AudioEngineProxy& aeProxy);
  bool handleRequest(const Glib::ustring& path, std::shared_ptr<NetworkRequest> request) override;
};
