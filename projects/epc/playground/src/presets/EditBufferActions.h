#pragma once

#include <proxies/audio-engine/AudioEngineProxy.h>
#include <http/ContentSection.h>
#include "http/RPCActionManager.h"

class EditBuffer;
class Parameter;

class EditBufferActions : public RPCActionManager
{
 private:
  typedef RPCActionManager super;

 public:
  typedef Parameter* tParameterPtr;
  explicit EditBufferActions(EditBuffer& editBuffer, AudioEngineProxy& aeProxy);
  ~EditBufferActions() override = default;
  EditBufferActions(const EditBufferActions& other) = delete;
  EditBufferActions& operator=(const EditBufferActions&) = delete;
  bool handleRequest(const Glib::ustring& path, std::shared_ptr<NetworkRequest> request) override;
};
