#pragma once
#include <string>
#include <device-settings/Settings.h>
#include <presets/EditBuffer.h>
#include <synth/C15Synth.h>
#include <proxies/audio-engine/AudioEngineProxy.h>

class XMLPresetLoader
{
 public:
  static void loadFirstPresetOfBank(Application *app, const std::string &fileName, C15Synth *synth);
  static void sendToAE(EditBuffer *eb, C15Synth *synth);
};
