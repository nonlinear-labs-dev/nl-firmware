#pragma once

class Preset;
class EditBuffer;

class ParameterPresenterRules
{
 public:
  static bool isPresetFXUnused_I(Preset* p);
  static bool isPresetFXUnused_II(Preset* p);

  static bool isSingleFXIUnused(EditBuffer& eb);
  static bool isSingleFXIIUnused(EditBuffer& eb);
};
