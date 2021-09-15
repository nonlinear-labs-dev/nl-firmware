#pragma once

#include <proxies/audio-engine/AudioEngineProxy.h>
#include <http/ContentSection.h>
#include <http/SectionAndActionManager.h>
#include "http/RPCActionManager.h"

class EditBuffer;
class Parameter;

class EditBufferActions : public SectionAndActionManager
{
 private:
  typedef SectionAndActionManager super;

 public:
  typedef Parameter* tParameterPtr;
  explicit EditBufferActions(UpdateDocumentContributor* parent, EditBuffer& editBuffer, AudioEngineProxy& aeProxy);
  ~EditBufferActions() override = default;
  EditBufferActions(const EditBufferActions& other) = delete;
  EditBufferActions& operator=(const EditBufferActions&) = delete;
};
