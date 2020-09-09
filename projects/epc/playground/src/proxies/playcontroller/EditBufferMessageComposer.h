#pragma once

#include <proxies/playcontroller/MessageComposer.h>

class Preset;

class EditBufferMessageComposer : public MessageComposer
{
 private:
  using super = MessageComposer;

 public:
  EditBufferMessageComposer();
};
