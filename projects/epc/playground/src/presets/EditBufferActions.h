#pragma once

#include <proxies/audio-engine/AudioEngineProxy.h>
#include "http/RPCActionManager.h"

class EditBuffer;
class Parameter;
class Settings;

class EditBufferActions : public RPCActionManager
{
 private:
  typedef RPCActionManager super;
  Settings& m_settings;

 public:
  typedef Parameter* tParameterPtr;
  explicit EditBufferActions(EditBuffer& editBuffer, AudioEngineProxy& aeProxy, Settings& settings);
  ~EditBufferActions() override = default;

  EditBufferActions(const EditBufferActions& other) = delete;
  EditBufferActions& operator=(const EditBufferActions&) = delete;
};
