#pragma once

#include <proxies/hwui/panel-unit/boled/InfoContent.h>
#include <presets/EditBuffer.h>

class SoundInfoContent : public InfoContent
{
 public:
  SoundInfoContent();
  void fillContents() override;
};