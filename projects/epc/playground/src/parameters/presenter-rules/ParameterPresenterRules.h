#pragma once
#include <nltools/Types.h>

class Preset;
class EditBuffer;
class Parameter;

class ParameterPresenterRules
{
 public:
  static bool isPresetFXUnused_I(Preset* p);
  static bool isPresetFXUnused_II(Preset* p);

  static bool isSingleFXIUnused(EditBuffer& eb);
  static bool isSingleFXIIUnused(EditBuffer& eb);

  static bool allowToggling(const Parameter* selected, const EditBuffer* editBuffer);

  static bool isDualPartMuted(VoiceGroup vg, const EditBuffer* eb);
};
