#include <xml/VersionAttribute.h>

VersionAttribute::VersionAttribute(int v)
    : Attribute("version", v)
{
}

VersionAttribute::~VersionAttribute()
{
}

VersionAttribute &VersionAttribute::get()
{
  static VersionAttribute currentFileVersion(getCurrentFileVersion());
  return currentFileVersion;
}

int VersionAttribute::getCurrentFileVersion()
{
  enum class History
  {
    Reaktor = 5,
    Version1_5 = 6,
    NAMM_2020 = 7,
    FIX_UNISON_VOICES = 8,
    DUAL_SPLIT_POINT = 9,
    DUAL_SPLIT_POINT_PRESETS = 10,
    HW_SEND_PARAMETERS = 11,
    MORE_MOD_PARAMS = 12
  };

  return static_cast<int>(History::MORE_MOD_PARAMS);
}
