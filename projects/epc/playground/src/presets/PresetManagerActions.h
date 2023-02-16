#pragma once

#include "http/RPCActionManager.h"
#include "use-cases/PresetManagerUseCases.h"
#include <libsoup/soup-message-body.h>
#include <use-cases/SoundUseCases.h>
#include <http/SectionAndActionManager.h>

namespace UNDO
{
  class Transaction;
}

class PresetManager;
class Preset;
class HTTPRequest;
class AudioEngineProxy;
class Settings;

class PresetManagerActions : public SectionAndActionManager
{
 public:
  explicit PresetManagerActions(UpdateDocumentContributor* parent, PresetManager& presetManager,
                                AudioEngineProxy& aeProxy, Settings& settings);
  bool handleRequest(const Glib::ustring& path, std::shared_ptr<NetworkRequest> request) override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;

 private:
  PresetManager& m_presetManager;
  Settings& m_settings;
  AudioEngineProxy& m_aeProxy;
};