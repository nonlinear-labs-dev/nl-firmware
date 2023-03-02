#pragma once

class Preset;

class PresetPresenter
{
 public:
  static bool isMonoActive(const Preset* p);
  static bool isUnisonActive(const Preset* p);
  static bool isScaleActive(const Preset* p);
};
