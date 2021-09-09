#pragma once
#include <string>
#include <device-settings/Settings.h>

class C15Synth;

class XMLPresetLoader
{
 public:
  static void loadTestPreset(C15Synth *synth, const std::string &subDir, const std::string &uuid);
  static void loadTestPresetFromBank(C15Synth* synth, const std::string& subDir, const std::string& bankFileName,
                                     Settings& settings);
};
