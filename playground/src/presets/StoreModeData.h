#pragma once
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/PresetBank.h>

struct StoreModeData {
public:
  StoreModeData();
  int bankPos = -1;
  int presetPos = -1;
};